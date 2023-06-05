#pragma once
#include "../../Events/EventSystem.h"
#include "../../Logger.h"
#include <vector>

#include "../VulkanEvents.h"
#include "Device.h"

namespace Landmark {
	namespace Vk {
		class Vulkan;
		class Task;

		class DeviceManager : Events::EventDispatcher {
			friend Vulkan;

			using DeviceID = uint64_t;


			static inline std::map<Device::DeviceID, Device> Devices;
			//std::vector<VkPhysicalDevice> PhysicalDevices;

			static inline Logger LOGGER = Logger("DeviceManager");

			void LogGpusInfo();



			static void InitializeTasks();

			static void EnumerateDevices();

		protected:
			static void Init();
		public:
			static const std::map<Device::DeviceID, Device>& GetDevices() { return Devices; }
		};





	}
}
