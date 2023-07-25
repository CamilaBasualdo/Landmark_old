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
					Name + " | Present: " + (device.PresentCapable ? "Yes" : "No"));
			}
			LOGGER.Log_List("Vulkan Devices Available:", _DeviceNames, Logger::yellow);
		}


		void DeviceManager::Init()
		{
			
			LOGGER.Log("Init");
			EnumerateDevices();

			InitializeDevices();
		}


		void DeviceManager::InitializeDevices()
		{
			Event_GpuTaskRequest e = Dispatch<Event_GpuTaskRequest>();
			auto Requests = e.GetRequests();


			for (auto request : Requests)
			{
				

				InitDevice(request);
			}

			Dispatch<Event_VulkanDeviceInit>();
		}

		void DeviceManager::InitDevice(
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

			std::vector<const char* > RequestedExtensions = {};
			std::vector<const char*> RequestedLayers = { "VK_LAYER_KHRONOS_validation" };



			for (auto& request: Requests.second)
			{
				LOGGER.Log("Checking Request " + request._ShallowRequest.Name);
				auto RequestedCapabilities = request._ShallowRequest.RequestedCapabilities;
				for (auto& ext : request._ShallowRequest.Extensions)
					RequestedExtensions.push_back(ext.data());
				for (auto& lay : request._ShallowRequest.Layers)
					RequestedLayers.push_back(lay.data());
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
						if (queueFamily.Usage.size() == queueFamily.TotalQueueCount) {
							LOGGER.Log("No more queues available in this family");
							continue;

						}

						//otherwise create new reservation
						queueFamily.Usage.push_back({ { request._task }, request._ShallowRequest._type });
						LOGGER.Log("Created new Reservation. Queues Remaining: " + std::to_string(queueFamily.TotalQueueCount- queueFamily.Usage.size()));
						break;

					}
					else
						LOGGER.Log("Is not compatible");
					i++;
				}
			}
			LOGGER.Log("Finished Allocating Tasks");

			std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
			std::vector<std::vector<float>> Queue_Priorities = {};
			for (int i = 0 ; i <QueueFamilies.size() ;i++)
			{
				LOGGER.Log("=== QueueFamily " + std::to_string(i) + " ===");
				auto& queueFamily = QueueFamilies[i];


				if (queueFamily.Usage.empty()) continue;
				
				for (int q = 0;q < queueFamily.Usage.size();q++)
				{
					auto& queue = queueFamily.Usage[q];
					std::string Log = "Queue " + std::to_string(q) + " Task Count: " + std::to_string(queue.Tasks.size()) + " Usage: " + std::to_string(queue.TotalUse) + "%";
					LOGGER.Log(Log);
					for (auto& task : queue.Tasks)
						LOGGER.Log(" | " + task->Name + " : " + std::to_string(task->taskType) + "%");
					


				}
				
				VkDeviceQueueCreateInfo createInfo;
				
				createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				createInfo.queueFamilyIndex = i;
				createInfo.queueCount = queueFamily.Usage.size();
				
				//std::vector<float> prior(createInfo.queueCount, 1.0f);
				//Queue_Priorities.push_back(prior);
				Queue_Priorities.emplace_back(createInfo.queueCount, 1.0f);

				createInfo.pQueuePriorities = Queue_Priorities[i].data();
				createInfo.flags = 0b0;
				createInfo.pNext = nullptr; //MAKE SURE TO CHANGE, WILL BREAK IF NOT
				//will it tho? i dont think you have to chain queue infos like this.
				//you do it in the array
				

				queue_create_infos.push_back(createInfo);
			}

			VkDeviceCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			
			createInfo.pQueueCreateInfos = queue_create_infos.data();
			createInfo.queueCreateInfoCount = queue_create_infos.size();


			createInfo.enabledLayerCount = RequestedLayers.size();
			createInfo.ppEnabledLayerNames = RequestedLayers.data();

			createInfo.enabledExtensionCount = RequestedExtensions.size();
			createInfo.ppEnabledExtensionNames = RequestedExtensions.data();

			VkPhysicalDeviceFeatures deviceFeatures{};
			createInfo.pEnabledFeatures = &deviceFeatures;

			Device::DeviceCreateInfo FullCreateInfo = { createInfo };

			for (int i = 0 ; i < QueueFamilies.size();i++)
			{
				auto& Family = QueueFamilies[i];
				int p = 0;
				for (auto& TaskCollection : Family.Usage)
				{
					
					Device::DeviceCreateInfo::QueueIdentifier qId = { i, p };
					
					FullCreateInfo.Tasks.insert(std::make_pair( qId,TaskCollection.Tasks));
						p++;
				}
				
			}

			//Device Creation
			LogicalDevices.emplace_back(&Device, FullCreateInfo);
			if (LogicalDevices.back()._PhysicalDevice->PresentCapable)
				MainPresentingDevice = &LogicalDevices.back();


			

		}
	}
}
