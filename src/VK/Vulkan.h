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
		class Vulkan :Module, Events::EventDispatcher {
			friend Engine;
			friend DeviceManager;
			static inline Debug::Logger LOGGER = Debug::Debugger::GetLogger("Vulkan");


			static inline VkInstance _VkInstance;
			static inline VkDebugUtilsMessengerEXT debugMessenger;
			Vulkan(VulkanInitParameters _p) :InitializationParameters(_p){

			}
			const VulkanInitParameters InitializationParameters;
			void InstanceInit();
		protected:
			static VkInstance& GetVkInstance() { return _VkInstance; }
			

			// Inherited via Module
			virtual std::string GetName() const override;
			virtual void PreInit() override;
			virtual void Init() override;
			virtual void PostInit() override;
		};


	}
}

