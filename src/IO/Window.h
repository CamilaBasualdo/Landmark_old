#pragma once
#include <list>
#include <vulkan/vulkan.hpp>

#include "../Logger.h"
#include "../VK/Devices/DeviceManager.h"
#include <General_Utils.h>
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
			uvec2 WindowSize = { 800,600 };

			GLFWwindow* _Window;
			const VkSurfaceKHR _Surface;
			VkSwapchainKHR swapChain;
			std::vector<VkImage> Images;
			std::vector<VkImageView> ImageViews;
			std::vector<VkFramebuffer> Framebuffers;

			VkSemaphore ImageAvailableSemaphore;
			VkFence ImageAquiringFence;
			bool Initialized = false;
			//VkPhysicalDevice _DeviceAssosiated;
			
			
			
		private:
			
			
		
		protected:
			Window();
			void Init();
			void Destroy();

		public:

			
			VkSurfaceKHR GetSurface() { return _Surface; };
			bool GetShouldClose();
			void SetShouldClose(bool state);

			VkFramebuffer GetNextImage(uint32_t* ImageIndex);
			VkSemaphore GetImageAvailableSemaphore() const { return ImageAvailableSemaphore; }

			void MakeCurrent();

			void Present(const std::vector<VkSemaphore>& WaitSemaphores, uint32_t ImageIndex);

			uvec2 GetWindowSize() const
			{
				return WindowSize;
			}

			
			bool operator==(const Window& o) const { return _Window == o._Window; }
		protected:


			void ResizeCallback( int sizeX, int sizeY);
			void FramebufferSizeCallback(int sizeX, int sizeY);
			void CloseCallback();

			
		};
		
	}
}
