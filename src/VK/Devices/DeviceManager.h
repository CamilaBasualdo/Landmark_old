#pragma once
#include "../../Events/EventSystem.h"
#include "../../Logger.h"
#include <vector>

#include "../VulkanEvents.h"
#include "Device.h"
#include "PhysicalDevice.h"

namespace Landmark {
	namespace Vk {
		class Vulkan;
		class Task;

		class DeviceManager : Events::EventDispatcher {
			friend Vulkan;

			using DeviceID = uint64_t;

			static inline std::vector<PhysicalDevice> PhysicalDevices;
			 
			//std::vector<VkPhysicalDevice> PhysicalDevices;

			static inline Logger LOGGER = Logger("DeviceManager");

			void LogGpusInfo();



			static void InitializeTasks();
			static void InitDeviceQueues(const std::pair<PhysicalDevice::PhysicalDeviceID, std::vector<Event_GpuTaskRequest::FullTaskRequest>>&);
			static void EnumerateDevices();

			
		protected:
			static void Init();
		public:
			static const std::vector<PhysicalDevice>& GetDevices() { return PhysicalDevices; }
			//static const std::map<Device::DeviceID, Device>& GetDevices() { return Devices; }
		};





	}
}
