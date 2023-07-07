#include "DeviceManager.h"

#include <iomanip>

#include "../VulkanEvents.h"
#include "../Vulkan.h"
#include <sstream>

namespace Landmark
{
	namespace Vk
	{
		void DeviceManager::EnumerateDevices()
		{
			uint32_t deviceCount = 0;
			vkEnumeratePhysicalDevices(Vulkan::GetVkInstance(), &deviceCount, nullptr);


			if (deviceCount == 0)
				LOGGER.Critical("No GPUs With Vulkan support found!", true);


			std::vector<VkPhysicalDevice> devices(deviceCount);
			vkEnumeratePhysicalDevices(Vulkan::GetVkInstance(), &deviceCount, devices.data());
			PhysicalDevices.reserve(deviceCount);
			PhysicalDevice::PhysicalDeviceID i = 0;
			for (auto device : devices)
			{
				PhysicalDevices.push_back(PhysicalDevice(device,i));
				i++;
			}

			//LOGGING
			std::vector<std::string> _DeviceNames;
			for (int i = 0; i < deviceCount; i++)
			{
				PhysicalDevice device = PhysicalDevices[i];
				_DeviceNames.push_back(
					DeviceTypes_toString(device._DeviceType) + " [" + std::to_string(device.DeviceID) + "] : " + device.
					Name + " | Present: " + std::to_string(device.PresentCapable));
			}
			LOGGER.Log_List("Vulkan Devices Available:", _DeviceNames, Logger::yellow);
		}


		void DeviceManager::Init()
		{
			LOGGER.Log("Init");
			EnumerateDevices();

			InitializeTasks();
		}


		void DeviceManager::InitializeTasks()
		{
			Event_GpuTaskRequest e = Dispatch<Event_GpuTaskRequest>();
			auto Requests = e.GetRequests();


			for (auto request : Requests)
			{
				

				InitDeviceQueues(request);
			}
		}

		void DeviceManager::InitDeviceQueues(
			const std::pair<PhysicalDevice::PhysicalDeviceID, std::vector<Event_GpuTaskRequest::FullTaskRequest>>& Requests)
		{
			
			PhysicalDevice& Device = PhysicalDevices[Requests.first];
			LOGGER.Log("Device Queues Init for " + Device.Name);
			struct QueueUsage
			{
				std::vector<Task*> Tasks;
				int TotalUse = 0;
			};
			struct QueueFamilyInfo
			{
				int TotalQueueCount;
				QueueFamily::QueueFamilyCapabilitiesMask Capabilities;
				bool Present;
				std::vector<QueueUsage> Usage;
			};

			std::vector<QueueFamilyInfo> QueueFamilies;
			
			for (auto& queueFamily : Device.QueueFamilies)
				QueueFamilies.push_back({ queueFamily.QueueCount,queueFamily.Capabilities,{} });
			for (int i = 0 ; i< QueueFamilies.size();i++)
			{
				auto& qf = QueueFamilies[i];
				LOGGER.Log("Queue " + std::to_string(i) + " : " + std::to_string(qf.TotalQueueCount) + " : " + std::to_string(qf.Capabilities));
			}

			/*
			store per queueFamily:
				1. number of queues
				2. Capabilities
				3. array of reseved queues {
					a) Usage
					b) Pointer to Tasks
				}

			 
			 */
			/*
				1. calculate how many queues are available for each requested capability
				2. Fill requests into best fit queues. Current Version is First Fit
				3. then attempt to do adding of two queues IF
					a) queue 1 mask AND queue 2 mask == 0
					b) Queue 1 usage + queue 2 usage <= 100%
			 */
			
			for (auto request: Requests.second)
			{
				LOGGER.Log("Checking Request " + request._ShallowRequest.Name);
				auto RequestedCapabilities = request._ShallowRequest.RequestedCapabilities;
				int i = 0;
				for (auto& queueFamily : QueueFamilies)
				{
					LOGGER.Log("Checking against " + std::to_string(i));
					bool ReservationFound = false;
					//AND requested vs available. if final result is equal to origina request, then it fits perfectly.
					QueueFamily::QueueFamilyCapabilitiesMask CompatabilityCheck = request._ShallowRequest.RequestedCapabilities & RequestedCapabilities;
					if (CompatabilityCheck == RequestedCapabilities)
					{
						LOGGER.Log("Is Compatible ");
						//for every reservation in this queue family
						for (auto& ReservedQueue : queueFamily.Usage)
						{
							//if this task fits in this reservation. aka usage does not exceed 100%
							if (ReservedQueue.TotalUse + request._ShallowRequest._type <= 100) //FITS!!!!
							{
								ReservationFound = true;
								ReservedQueue.Tasks.push_back(request._task);
								ReservedQueue.TotalUse += request._ShallowRequest._type;
								break;

							}
							LOGGER.Log("Does not fit existing reservation ");
						}
						if (ReservationFound) break;
						//does not fit in any existing reservation

						//if queue family is at capacity skip to next family
						if (queueFamily.Usage.size() == queueFamily.TotalQueueCount) continue;


						//otherwise create new reservation
						queueFamily.Usage.push_back({ { request._task }, request._ShallowRequest._type });
						LOGGER.Log("Created new Reservation ");
						break;

					}
					else
						LOGGER.Log("Is not compatible");
					i++;
				}
			}
			LOGGER.Log("Finished Allocating Tasks");

			std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
			for (int i = 0 ; i <QueueFamilies.size() ;i++)
			{
				LOGGER.Log("QueueFamily " + std::to_string(i));
				auto& queueFamily = QueueFamilies[i];
				for (int q = 0;q < queueFamily.Usage.size();q++)
				{
					auto& queue = queueFamily.Usage[q];
					std::string Log = "	Queue " + std::to_string(q) + " Task Count: " + std::to_string(queue.Tasks.size()) + " Usage:" + std::to_string(queue.TotalUse) + "%";
					LOGGER.Log(Log);
					for (auto& task : queue.Tasks)
						LOGGER.Log(" | " + task->Name + " : " + std::to_string(task->taskType) + "%");
					


				}
				VkDeviceQueueCreateInfo createInfo;
				createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				createInfo.queueFamilyIndex = i;
				createInfo.queueCount = queueFamily.Usage.size();
				float queuePriority = 1.0f;
				createInfo.pQueuePriorities = &queuePriority;
				
				

				queue_create_infos.push_back(createInfo);
			}

			VkDeviceCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

			createInfo.pQueueCreateInfos = queue_create_infos.data();
			createInfo.queueCreateInfoCount = queue_create_infos.size();
			VkPhysicalDeviceFeatures deviceFeatures{};
			createInfo.pEnabledFeatures = &deviceFeatures;

		}
	}
}
