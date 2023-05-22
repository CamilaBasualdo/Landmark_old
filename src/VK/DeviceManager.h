#pragma once
#include "../Events/EventSystem.h"
#include "../Debug/Debug.h"
namespace Landmark {
	namespace Vk {
		class Vulkan;
		class DeviceManager : Events::EventDispatcher{
			friend Vulkan;
			static inline Debug::Logger LOGGER = Debug::Debugger::GetLogger("DeviceManager");
		protected:
			static void Init();
		};
	}
}
