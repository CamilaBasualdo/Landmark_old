#pragma once

#include <iostream>
#include "../Events/EventSystem.h"
#include "VulkanEvents.h"
#include "../Module/Module.h"
namespace Landmark
{
	class Engine;
	namespace Vk
	{
		class DeviceManager;
		struct VulkanInitParameters {
			const char* AppName = nullptr;
			bool ValidationMode = false;
		};
		class Vulkan : Events::EventDispatcher {
			friend Engine;
			friend DeviceManager;
			static inline Debug::Logger LOGGER = Debug::Debugger::GetLogger("Vulkan");


			static inline VkInstance _VkInstance;
			static inline VkDebugUtilsMessengerEXT debugMessenger;
			Vulkan() {
				std::cout << "teste";

				DispatchEvent<Event_VulkanQueuesPreInit>();
				//DispatchEvent<Vulkan>();
			}

			static void InstanceInit(const VulkanInitParameters param);
		protected:
			static VkInstance& GetVkInstance() { return _VkInstance; }
			static void Init(const VulkanInitParameters param);
		};


	}
}

