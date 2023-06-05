#include "DeviceManager.h"

#include "../VulkanEvents.h"
#include "../Vulkan.h"

namespace Landmark
{
	namespace Vk
	{

		void DeviceManager::EnumerateDevices()
		{

			uint32_t deviceCount = 0;
			vkEnumeratePhysicalDevices(Vulkan::GetVkInstance(), &deviceCount, nullptr);
			std::vector<VkPhysicalDevice> devices(deviceCount);
			vkEnumeratePhysicalDevices(Vulkan::GetVkInstance(), &deviceCount, devices.data());

			if (deviceCount == 0)
				LOGGER.Critical("No GPUs With Vulkan support found!", true);

			std::vector<std::string> _DeviceNames;
			for (int i = 0; i < deviceCount; i++)
			{
				Device d(devices[i]);
				_DeviceNames.push_back(Device::DeviceTypes_toString(d.DeviceType) + " [" + std::to_string(d.ID) + "] : " + d.Name);
				//LOGGER.Log("Found " + Device::DeviceTypes_toString(d.DeviceType) + " Device [" + std::to_string(d.ID) + "] : " + d.Name, Logger::yellow);
				Devices.emplace(std::make_pair(d.ID, d));
			}
			LOGGER.Log_List("Vulkan Devices Available:", _DeviceNames,Logger::yellow);
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


			for (auto DeviceRequests : Requests) {
				std::vector<std::string> DeviceRequestsLog = {};

				//8 graphics + present queues == [0b11] -> 800

				//for every task
				auto DeviceCapabilities = Devices.at(DeviceRequests.first).QueueCapability;
				std::map<Task::CapabilitiesMask, int>CapacityRequested;
				for (auto Request : DeviceRequests.second) {

					auto Capabilities = Request._ShallowRequest.RequestedCapabilities;
					

					//Logging Info
					{
						std::string RequestInfo = "";
						RequestInfo += "" + Request._ShallowRequest.Name + ":";


						RequestInfo += "\n	Intensity: ";
						switch (Request._ShallowRequest._type) {
						case Task::INVALID: break;
						case Task::VERY_HIGH: RequestInfo += "Very High"; break;
						case Task::HIGH: RequestInfo += "High"; break;
						case Task::MEDIUM:RequestInfo += "Medium"; break;
						case Task::LOW:RequestInfo += "Low"; break;
						default:;
						}
						RequestInfo += "\n	Capabilities: ";

						if (Capabilities & Task::PRESENT) RequestInfo += "Present | ";
						if (Capabilities & Task::GRAPHICS) RequestInfo += "Graphics | ";
						if (Capabilities & Task::COMPUTE) RequestInfo += "Compute | ";
						if (Capabilities & Task::TRANSFER) RequestInfo += "Transfer | ";
						if (Capabilities & Task::SPARSE_BINDING) RequestInfo += "Sparse Binding | ";

						DeviceRequestsLog.push_back(RequestInfo);
					}

					if (!CapacityRequested.contains(Capabilities))
						CapacityRequested[Capabilities] = 0;


					CapacityRequested[Capabilities] += static_cast<int>(Request._ShallowRequest._type);


					
				}

				//LOGGING
				{
					LOGGER.Log_List("Device " + std::to_string(DeviceRequests.first) +
						" (" + std::string((Devices.at(DeviceRequests.first).Name)) + ") Requests:",
						DeviceRequestsLog);
					for (auto Re : CapacityRequested)
					{
						LOGGER.Log("Queue Capability: " + std::to_string(Re.first) + " usage: " + std::to_string(Re.second) + "%");
					}
				}
				std::vector<std::string> CapabilitiesList;
				for (auto Capability : DeviceCapabilities)
				{
					std::string CapabilityLog = "(" + std::to_string(Capability.second/100) + ") | " + std::to_string(Capability.second) + "%";

					CapabilityLog += "| ";
					if (Capability.first & Task::GRAPHICS) CapabilityLog += "GRAPHICS | ";
					if (Capability.first & Task::COMPUTE) CapabilityLog += "COMPUTE | ";
					if (Capability.first & Task::PRESENT) CapabilityLog += "PRESENT | ";
					if (Capability.first & Task::TRANSFER) CapabilityLog += "TRANSFER | " ;
					if (Capability.first & Task::SPARSE_BINDING) CapabilityLog += "SPARSE_BINDING | ";

					CapabilitiesList.push_back(CapabilityLog);
				}
				LOGGER.Log_List(Devices.at(DeviceRequests.first).Name + " | Queue Capabilities", CapabilitiesList);


			}


		}

	}
}

