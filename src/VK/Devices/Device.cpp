#include "Device.h"
#include "../../IO/WindowManager.h"

namespace Landmark
{
	namespace Vk
	{
		std::string Device::DeviceTypes_toString(DeviceTypes _type)
		{
			switch (_type)
			{
			case CPU: return "Cpu";
			case INTEGRATED: return "Integrated";
			case DISCRETE: return "Discrete";
			case VIRTUAL: return "Virtual";
			case OTHER: return "Other";
			default: ;
			}
		}

		VkQueue Device::GetQueue(int Family, int QueueIndex ) const
		{
			VkQueue q;
			
			vkGetDeviceQueue(_LogicalDevice, Family, QueueIndex, &q);
			
			return q;
		}

		Device::Device(PhysicalDevice* _PhysicalDevice, DeviceCreateInfo createInfo) : _PhysicalDevice(_PhysicalDevice)
		{
			//VkDeviceCreateInfo ci = createInfo;
			//VkPhysicalDevice phy = 
			if (vkCreateDevice(_PhysicalDevice->GetVkDevice() , &createInfo.createinfo, nullptr,&_LogicalDevice) != VK_SUCCESS) {
				LOGGER.Critical("Logical Device Creation Failed (" + _PhysicalDevice->Name + ")", true);
				//throw std::runtime_error("failed to create logical device!");
			}
			//LOGGER.Log("Vulkan Device (" + _PhysicalDevice->Name + ") Initialized");

			//ActiveQueuesFamilies.resize(createInfo.createinfo.queueCreateInfoCount);

			for (int i = 0 ; i < createInfo.createinfo.queueCreateInfoCount;i++)
			{
				auto queueInfo = createInfo.createinfo.pQueueCreateInfos[i];
				int queueFamilyId = queueInfo.queueFamilyIndex;
				//ActiveQueuesFamilies[queueFamilyId].reserve(queueInfo.queueCount);
				for (int p = 0;p<queueInfo.queueCount;p++)
				{
					
					//ActiveQueuesFamilies[queueFamilyId].push_back(Queue(this, queueFamilyId, p,createInfo.Tasks[std::make_pair(queueFamilyId,p)]));
					ActiveQueuesFamilies[queueFamilyId].emplace_back(this, queueFamilyId, p, createInfo.Tasks[std::make_pair(queueFamilyId, p)]);
					//vkGetDeviceQueue(_LogicalDevice, i, p, &Queues[i][p]);
				}
			}
			LOGGER.Log("Vulkan Device (" + _PhysicalDevice->Name + ") Initialized, Queues & Tasks Initialized");
		}


		
	}
}
