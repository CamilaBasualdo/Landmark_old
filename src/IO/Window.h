#pragma once
#include <list>
#include <vulkan/vulkan.hpp>

#include "../Logger.h"
#include "../VK/Devices/DeviceManager.h"
class GLFWwindow;
namespace Landmark
{
	namespace Vk { class Device; class DeviceManager; }
	namespace IO
	{
		class WindowManager;
		class Window
		{
			friend WindowManager;
			friend Vk::DeviceManager;
			friend Vk::Device;

			static inline Logger LOGGER = Logger("Window");


			enum Type
			{
				
			};

			GLFWwindow* _Window;
			const VkSurfaceKHR _Surface;

			

			VkSwapchainKHR swapChain;
			std::vector<VkImage> Images;
			std::vector<VkImageView> ImageViews;
			std::vector<VkFramebuffer> Framebuffers;

			bool Initialized = false;
			//VkPhysicalDevice _DeviceAssosiated;
			
			
			
		private:
			Window();
		
		protected:
			void Init();

		public:
			
			VkSurfaceKHR GetSurface() { return _Surface; };
			bool GetShouldClose();
			void SetShouldClose(bool state);

			void SwapBuffers();
			

			void MakeCurrent();

			
		};
		
	}
}
