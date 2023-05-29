#include "RenderLogic.h"
#include <vulkan/vk_enum_string_helper.h>
namespace Landmark
{
	namespace Render
	{
		RenderSystem::RenderSystem()
		{
			
			
		}
	
		std::string RenderSystem::GetName() const
		{
			return std::string("RenderLogic");
		}

		void RenderSystem::PreInit()
		{
			LOGGER.Log("Init");
			
			



			SubscribeTo<Vk::Event_GpuTaskRequest>([&](Vk::Event_GpuTaskRequest& e) {
				//LOGGER.Log("Requesting Main Rendering Task on Device 0 (HARDCODED) Type: " + std::string(string_VkPhysicalDeviceType(e.AvailableDevices[0].deviceProperties.deviceType)));
				

				Vk::Event_GpuTaskRequest::TaskRequest request = {
					"RenderLogic Main Rendering Task",
					Vk::Task::TaskTypes::CONTINUOUS,
					e.AvailableDevices[0].ID,
					Vk::Task::GRAPHICS | Vk::Task::COMPUTE | Vk::Task::TRANSFER | Vk::Task::SPARSE_BINDING,
					{VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME},
					{}

				};

				RenderingTask = e.DeclareTask(request);
				});
			
		}
		void RenderSystem::Init() 		
		{
		}
		void RenderSystem::PostInit() 
		{
		}
	}
}
