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
		

		class Vulkan :Module, Events::EventDispatcher, Events::EventSubscriber {
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
			
			

			
			
			virtual void PreInit() override;
			virtual void Init() override;
			virtual void PostInit() override;

		public:
			static VkInstance& GetVkInstance() { return _VkInstance; }
			void Update() override;
		};


	}
}

