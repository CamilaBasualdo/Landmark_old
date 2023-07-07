#pragma once
#include <list>
#include <vulkan/vulkan.hpp>

#include "../Logger.h"
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
			//VkPhysicalDevice _DeviceAssosiated;
			
			
			
		private:
			Window();
		protected:
			
		public:
			
			VkSurfaceKHR GetSurface() { return _Surface; };
			bool GetShouldClose();
			void SetShouldClose(bool state);

			void SwapBuffers();
			

			void MakeCurrent(); 
		};
		
	}
}
