#include "Renderer.h"
#include <vulkan/vk_enum_string_helper.h>
#include "../IO/Window.h"
#include <thread>
#include "../Threads/ThreadManager.h"
#include "../Landmark.h"
#include "../IO/WindowManager.h"

#include <sstream>
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

		void Renderer::RenderingThreadStart()
		{
			
			LOGGER.Log("Rendering Thread Init");
			//ainWindow->MakeCurrent();

			while (!CloseThread)
			{
				if (IO::WindowManager::MainWindow().GetShouldClose())
					Engine::Shutdown();
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
