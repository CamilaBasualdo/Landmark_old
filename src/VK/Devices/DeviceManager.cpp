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


			Event_GpuTaskRequest e = Dispatch<Event_GpuTaskRequest>();

		}




		void DeviceManager::InitializeTasks(Event_GpuTaskRequest e)
		{

			auto Requests = e.GetRequests();


			for (auto DeviceRequests : Requests) {
				std::vector<std::string> DeviceRequestsLog = {};

				for (auto Request : DeviceRequests.second) {
					std::string RequestInfo = "";
					RequestInfo += "Request for (" + Request._ShallowRequest.Name + ")";


					RequestInfo += "\n	Type: ";
					switch (Request._ShallowRequest._type) {
					case Task::INVALID: break;
					case Task::CONTINUOUS: RequestInfo += "Continuous"; break;
					case Task::BURST: RequestInfo += "Burst"; break;
					case Task::SLEEPER:RequestInfo += "Sleeper"; break;
					default:;
					}
					RequestInfo += "\n	Capabilities: ";
					auto Capabilities = Request._ShallowRequest.RequestedCapabilities;
					if (Capabilities & Task::GRAPHICS) RequestInfo += "Graphics | ";
					if (Capabilities & Task::COMPUTE) RequestInfo += "Compute | ";
					if (Capabilities & Task::TRANSFER) RequestInfo += "Transfer | ";
					if (Capabilities & Task::SPARSE_BINDING) RequestInfo += "Sparse Binding | ";



					DeviceRequestsLog.push_back(RequestInfo);
				}

				//LOGGER.Log_List("Device " + std::to_string(DeviceRequests.first) + " (" + std::string(Devices[DeviceRequests.first]._Info.Name) + ") Requests:", DeviceRequestsLog);
			}


		}

	}
}

