#pragma once
#include <map>
#include <string>
#include <vulkan/vulkan.hpp>
#include <vector>
#include "Extension.h"
#include "Layer.h"
#include "../Task.h"
namespace Landmark
{
	namespace Vk
	{
		
		
			class DeviceManager;
			class Device
			{
				friend DeviceManager;
				
			public:
				using DeviceID = uint64_t;
				enum DeviceTypes
				{
					CPU = VK_PHYSICAL_DEVICE_TYPE_CPU,
					INTEGRATED = VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU,
					DISCRETE = VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU,
					VIRTUAL = VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU,
					OTHER = VK_PHYSICAL_DEVICE_TYPE_OTHER
				};

				static std::string DeviceTypes_toString(DeviceTypes _type);
				


				const VkPhysicalDevice PhysicalDevice;
				const VkPhysicalDeviceProperties Properties;
				const VkPhysicalDeviceFeatures Features;

				const DeviceID ID;
				const DeviceTypes DeviceType;
				const std::string Name;

				const std::vector<Extension> AvailableExtensions;
				const std::vector<Layer> AvailableLayer;

				const std::map<Task::CapabilitiesMask, int> QueueCapability;

			private:
				std::vector<Task*> Tasks;

				const std::vector<Extension> GetExtensions();
				const std::vector<Layer> GetLayers();
				const std::map<Task::CapabilitiesMask, int> GetQueueCapacities();

			protected:
				Device(VkPhysicalDevice _PhysicalDevice);
				
			};

		
		
		

	}
}
