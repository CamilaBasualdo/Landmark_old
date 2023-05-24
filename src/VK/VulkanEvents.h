#pragma once
#include "../Events/Event.h"
#include <vulkan/vulkan.h>
namespace Landmark
{
	namespace Vk
	{
		class Event_VulkanInstancePreInit : Events::Event {
		public:

		};

		class Event_VulkanQueuesPreInit : Events::Event
		{
		public:
			void RequestQueue(VkQueue* queue);
		};



		class Event_VulkanPhysicalDevicesPreInit : Events::Event {
			
		public:

			struct AvailableDeviceDetails {
				const VkPhysicalDeviceProperties deviceProperties;
				const VkPhysicalDeviceFeatures deviceFeatures;
				const std::vector<VkExtensionProperties> deviceExtensions;
				const std::vector<VkLayerProperties> deviceLayers;
			};
			const std::vector<AvailableDeviceDetails> AvailableDevices;
			Event_VulkanPhysicalDevicesPreInit(std::vector<AvailableDeviceDetails> _devices) :AvailableDevices(_devices) {

			}
		};
	}
}