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
			enum Type
			{
				
			};

			GLFWwindow* _Window;
			const VkSurfaceKHR _Surface;
			
			static inline Logger LOGGER = Logger("Window");

		private:
			Window();
		protected:
			VkSurfaceKHR GetSurface() { return _Surface; };
		public:
			

			bool GetShouldClose();
			void SetShouldClose(bool state);

			void SwapBuffers();
			static void PollEvents();

			void MakeCurrent(); 
		};
		
	}
}
