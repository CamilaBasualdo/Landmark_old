#include "Renderer.h"
#include <vulkan/vk_enum_string_helper.h>
#include "../IO/Window.h"
#include <thread>
#include "../Threads/ThreadManager.h"
#include "../Landmark.h"
#include "../IO/WindowManager.h"
#include "../VK/Devices/DeviceManager.h"
#include <sstream>
#include <fstream>
#include "Components/TestRenderComponent.h"
namespace Landmark
{
	class Engine;
}

namespace Landmark
{
	namespace Render
	{
		Renderer::Renderer()
		{
			
			
		}
	
		std::string Renderer::GetName() const
		{
			return std::string("RenderLogic");
		}

		void Renderer::PreInit()
		{
			
			
			



			SubscribeTo<Vk::Event_GpuTaskRequest>([&](Vk::Event_GpuTaskRequest& e) {
				//LOGGER.Log("Requesting Main Rendering Task on Device 0 (HARDCODED) Type: " + std::string(string_VkPhysicalDeviceType(e.AvailableDevices[0].deviceProperties.deviceType)));
				

				Vk::Event_GpuTaskRequest::TaskRequest request = {
					"Renderer Main Task",
					Vk::Task::TaskIntensities::VERY_HIGH,
					e.AvailableDevices[0].DeviceID,
					Vk::GRAPHICS | Vk::COMPUTE | Vk::TRANSFER | Vk::SPARSE_BINDING,
					{VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
						VK_KHR_SPIRV_1_4_EXTENSION_NAME,VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME,
						VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
						VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME, VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME, VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME},
					{}
				};

				RenderingTask = e.DeclareTask(request);
				});
			
			SubscribeTo<IO::Event_WindowFormatsSelected>([&](IO::Event_WindowFormatsSelected&)
			{
					LOGGER.Log("Window Manager Finsished selecting formats. Initializing");
					InitRenderPass();
					LOGGER.Log("Render Pass Created");
					Dispatch<Event_RendererInit>();
					CreateDefaultGBufferShader();
					RenderingThread = std::thread(RenderingThreadStart);
					LOGGER.Log("Rendering Thread Dispatched");
			});
			
			///RenderingThread.join();
			
		}
		void Renderer::Init() 		
		{
			
		}
		void Renderer::PostInit() 
		{

		}
		void Renderer::InitRenderPass()
		{
			LOGGER.Log("DeviceManager Finished, ");


			auto& Mainwindow = IO::WindowManager::MainWindow();
			VkAttachmentDescription colorAttachment{};
			
			colorAttachment.format = IO::WindowManager::GetSurfaceFormat().format;
			colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

			colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

			colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

			colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

			VkAttachmentReference colorAttachmentRef{};
			colorAttachmentRef.attachment = 0;
			colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

			//std::vector<VkSubpassDescription> SubPasses;
			VkSubpassDescription subpass{};
			subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			
			
			subpass.colorAttachmentCount = 1;
			subpass.pColorAttachments = &colorAttachmentRef;

			VkRenderPassCreateInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			renderPassInfo.attachmentCount = 1;
			renderPassInfo.pAttachments = &colorAttachment;
			renderPassInfo.subpassCount = 1;
			renderPassInfo.pSubpasses = &subpass;


			if (vkCreateRenderPass(Vk::DeviceManager::GetMainPresentingDevice()->GetVkDevice() , &renderPassInfo, nullptr, &RenderPass) != VK_SUCCESS) {
				LOGGER.Critical("Failed to create Renderpass");
			}
			std::stringstream loginfo;
			loginfo<< "RenderPass Created. " << renderPassInfo.subpassCount << " Subpasses. ";
			for (int i = 0; i < renderPassInfo.subpassCount; i++)
				loginfo <<"\n" << i << " " << string_VkPipelineBindPoint(renderPassInfo.pSubpasses[i].pipelineBindPoint);
			LOGGER.Log( loginfo.str());
		}
		static std::vector<char> readFile(const std::string& filename) {
			std::ifstream file(filename, std::ios::ate | std::ios::binary);

			if (!file.is_open()) {
				throw std::runtime_error("failed to open file!");
			}
			size_t fileSize = (size_t)file.tellg();
			std::vector<char> buffer(fileSize);
			file.seekg(0);
			file.read(buffer.data(), fileSize);
			file.close();

			return buffer;
		}
		void Renderer::CreateDefaultGBufferShader()
		{
			

			DefaultGBufferShader
				.AttachModule(Vk::GraphicsPipeline::VERTEX, readFile("Deferred.vert.spv"))
				.AttachModule(Vk::GraphicsPipeline::FRAGMENT, readFile("Deferred.frag.spv"))
				.Build();
		}

		void Renderer::Render()
		{
			auto& window = IO::WindowManager::MainWindow();
			RenderingTask->BeginRecord();

			auto cmdBuffer = RenderingTask->GetCmdBuffer();
			VkRenderPassBeginInfo info;
			info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			info.renderPass = RenderPass;
			info.clearValueCount = 1;
			VkClearValue clearColor = { 0.0f,0.0f,0.0f,1.0f };
			info.pClearValues = &clearColor;
			info.pNext = nullptr;
			uint32_t imageindex = window.GetNextImageIndex();
			info.framebuffer = window.GetFramebuffer(imageindex);
			info.renderArea.offset = { 0,0 };
			auto size = window.GetWindowSize();
			info.renderArea.extent = { size.x,size.y };
			vkCmdBeginRenderPass(cmdBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);

			VkViewport vp;
			vp.x = 0;
			vp.y = 0;
			vp.width = size.x;
			vp.height = size.y;
			vp.minDepth = 0.0f;
			vp.maxDepth = 1.0f;
			vkCmdSetViewport(cmdBuffer,0,1, &vp);

			VkRect2D scissor;
			scissor.offset = { 0,0 };
			scissor.extent = { size.x,size.y };
			vkCmdSetScissor(cmdBuffer, 0, 1, &scissor);

			vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, DefaultGBufferShader.GetPipeline());
			vkCmdDraw(cmdBuffer, 3, 1, 0, 0);
			for (auto& comp : TestRenderComponent::STORAGE)
			{
				comp.Render();
			}
			vkCmdEndRenderPass(cmdBuffer);
			RenderingTask->EndRecord();
			window.PushFramebuffer(imageindex);
		}


		void Renderer::RenderingThreadStart()
		{
			
			LOGGER.Log("Rendering Thread Init");
			//ainWindow->MakeCurrent();

			while (!CloseThread)
			{
				if (IO::WindowManager::MainWindow().GetShouldClose())
					Engine::Shutdown();

				Render();
				
			}
		}

		
		void Renderer::Exit()
		{
			CloseThread = true;
			RenderingThread.join();
			LOGGER.Log("Rendering Thread Closed");
		}
	}
}
