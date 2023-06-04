#pragma once
#include <list>
#include <vulkan/vulkan.hpp>

#include "../Logger.h"
class GLFWwindow;
namespace Landmark
{
	namespace IO
	{
		class WindowManager;
		class Window
		{
			friend WindowManager;
			enum Type
			{
				
			};

			GLFWwindow* _Window;
			const VkSurfaceKHR _Surface;
			
			static inline Logger LOGGER = Logger("Window");

		private:
			Window();
		public:
			

			bool GetShouldClose();
			void SetShouldClose(bool state);

			void SwapBuffers();
			static void PollEvents();
		};
		
	}
}
