#pragma once
#include <vulkan/vulkan.hpp>
#include <iostream>
#include "../Events/EventSystem.h"
#include "VulkanEvents.h"
#include "../Module/Module.h"
#include "../Logger.h"
namespace Landmark
{
	class Engine;
	namespace IO { class Window; }
	namespace Vk
	{
		 class DeviceManager;
		

		class Vulkan :Module, Events::EventDispatcher {
			friend Engine;

			friend DeviceManager;
			friend IO::Window;
			static inline Logger LOGGER = Logger("Vulkan");


			static inline VkInstance _VkInstance;

			static inline VkDebugUtilsMessengerEXT debugMessenger;



			Vulkan() {

			}
			
			void InstanceInit();
		protected:
			static VkInstance& GetVkInstance() { return _VkInstance; }
			

			// Inherited via Module
			virtual std::string GetName() const override;
			virtual void PreInit() override;
			virtual void Init() override;
			virtual void PostInit() override;

		public:
			void Update() override;
		};


	}
}

