#include "Window.h"
#include "../VK/Vulkan.h"

#include <GLFW/glfw3.h>
Landmark::IO::Window::Window() : _Window([]()
{
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		auto Window = glfwCreateWindow(800, 600, "Default Window Title", NULL, NULL);
		if (!Window)
			LOGGER.Error("Failed to create GLFW Window");
		else
			LOGGER.Log("Window Created");
		return Window;
}()), _Surface([this]()
{
		VkSurfaceKHR surface;
		VkResult Result = glfwCreateWindowSurface(Vk::Vulkan::GetVkInstance(), _Window, nullptr, &surface);
		if (Result != VK_SUCCESS)
			LOGGER.Error("Failed to create Window Surface");
		else
			LOGGER.Log("Vulkan Surface Created");

	
		return surface;

}())
/*, _DeviceAssosiated([this]()
{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(Vk::Vulkan::GetVkInstance() , & deviceCount, nullptr);

		// Allocate memory for physical devices
		std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
		vkEnumeratePhysicalDevices(Vk::Vulkan::GetVkInstance(), &deviceCount, physicalDevices.data());

		// Find the physical device that supports the surface
		for (const auto& device : physicalDevices) {
			VkBool32 surfaceSupported = VK_FALSE;
			//vkGetPhysicalDeviceSurfaceSupportKHR(device, queueFamilyIndex, surface, &surfaceSupported);

			if (surfaceSupported == VK_TRUE) {
				return device;
			}
		}
}())*/
{
	
}

bool Landmark::IO::Window::GetShouldClose()
{
	return glfwWindowShouldClose(_Window);
}

void Landmark::IO::Window::SetShouldClose(bool state)
{
	glfwSetWindowShouldClose(_Window, state);
}

void Landmark::IO::Window::SwapBuffers()
{
	glfwSwapBuffers(_Window);
}



void Landmark::IO::Window::MakeCurrent()
{
	glfwMakeContextCurrent(_Window);
}
