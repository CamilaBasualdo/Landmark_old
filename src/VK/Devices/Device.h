#pragma once
#include <map>
#include <string>
#include <vulkan/vulkan.hpp>
#include <vector>

#include "../Task.h"
#include "PhysicalDevice.h"
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
				
				PhysicalDevice* _PhysicalDevice;
					
				


			private:
				std::vector<Task*> Tasks;

				

			protected:
				Device(PhysicalDevice* _PhysicalDevice);
				
			};

		
		
		

	}
}
