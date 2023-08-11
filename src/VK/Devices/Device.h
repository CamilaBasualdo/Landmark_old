#pragma once
#include <map>
#include <string>
#include <vulkan/vulkan.hpp>
#include <vector>
#include "Queue.h"
#include "../Task.h"
#include "PhysicalDevice.h"
#include <list>
namespace Landmark
{
	namespace Vk
	{
		
		
			class DeviceManager;
			class Queue;
			class Device
			{
				friend DeviceManager;
				friend Queue;
				friend Task;
				
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

			private:
				static inline Logger LOGGER = Logger("Vk::Device");
				PhysicalDevice* _PhysicalDevice;
				VkDevice _LogicalDevice;

				//sorted by [family][index]
				using _QueueFamiliy = std::list<Queue>;
				std::map<uint32_t, _QueueFamiliy> ActiveQueuesFamilies;

				std::vector<Task*> Tasks;

			protected:
				

			public:
				const PhysicalDevice& GetPhysicalDevice() { return *_PhysicalDevice; }
				VkDevice GetVkDevice() const { return _LogicalDevice; }
				VkQueue GetQueue(int QueueFamily, int QueueIndex) const;
				struct DeviceCreateInfo
				{
					VkDeviceCreateInfo createinfo;
					using QueueIdentifier = std::pair<uint32_t, uint32_t>;
					std::map<QueueIdentifier, std::vector<Task*>> Tasks;

				};
				Device(PhysicalDevice* _PhysicalDevice, DeviceCreateInfo createInfo);
				
			};

		
		
		

	}
}
