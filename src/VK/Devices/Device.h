#pragma once
#include <string>
#include <vulkan/vulkan.hpp>
#include <vector>
#include "Extension.h"
#include "Layer.h"
namespace Landmark
{
	namespace Vk
	{
		class Task;
		
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
			private:
				std::vector<Task*> Tasks;
			protected:
				Device(VkPhysicalDevice _PhysicalDevice);
				
			};

		
		
		

	}
}
