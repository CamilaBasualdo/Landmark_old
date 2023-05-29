#pragma once
#include "../../Events/EventSystem.h"
#include "../../Debug/Debug.h"
#include <vector>
#include <vulkan/vulkan.h>
#include "../VulkanEvents.h"
#include "Device.h"

namespace Landmark {
	namespace Vk {
		class Vulkan;
		class Task;
		namespace Devices
		{
			class DeviceManager : Events::EventDispatcher {
				friend Vulkan;

				using DeviceID = uint64_t;


				static inline std::map<Devices::Device::DeviceID, Devices::Device> Devices;
				//std::vector<VkPhysicalDevice> PhysicalDevices;

				static inline Debug::Logger LOGGER = Debug::Debugger::GetLogger("DeviceManager");

				void LogGpusInfo();



				void InitializeTasks(Event_GpuTaskRequest e);

				static void EnumerateDevices();

			protected:
				static void Init();
			public:
				static const std::map<Devices::Device::DeviceID, Devices::Device>& GetDevices() { return Devices; }
			};
		}




	}
}
