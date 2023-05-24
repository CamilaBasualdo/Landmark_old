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
			
			SubscribeTo<Vk::Event_VulkanQueuesPreInit>([&](Vk::Event_VulkanQueuesPreInit& e) {
				
				});



			SubscribeTo<Vk::Event_VulkanPhysicalDevicesPreInit>([&](Vk::Event_VulkanPhysicalDevicesPreInit& e) {
				LOGGER.Log("Asking for Devices");
				LOGGER.Log(std::string(string_VkPhysicalDeviceType(e.AvailableDevices[0].deviceProperties.deviceType)));
				std::string EXtensions = "Device Extensions: \n";
				for (auto ext : e.AvailableDevices[0].deviceExtensions)
					EXtensions += std::string(" | ") + ext.extensionName + std::string(" \n");
				LOGGER.Log(EXtensions);
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
