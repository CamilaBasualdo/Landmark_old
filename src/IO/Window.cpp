#include "Window.h"
#include "../VK/Vulkan.h"
#include "../VK/Devices/DeviceManager.h"
#include <GLFW/glfw3.h>
#include <vulkan/vk_enum_string_helper.h>
#include "WindowManager.h"
#include "../Renderer/Renderer.h"
Landmark::IO::Window::Window() : _Window([&]()
	{
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		auto Window = glfwCreateWindow(WindowSize.x, WindowSize.y, "Default Window Title", NULL, NULL);
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
	if (Vk::DeviceManager::GetMainPresentingDevice())
		Init();
}



		void Landmark::IO::Window::Init()
		{
			if (Initialized) return;


			auto PresentingDevice = Vk::DeviceManager::GetMainPresentingDevice();

			VkSwapchainCreateInfoKHR createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			createInfo.surface = _Surface;

			createInfo.minImageCount = WindowManager::GetImageCount();
			createInfo.imageFormat = WindowManager::GetSurfaceFormat().format;
			createInfo.imageColorSpace = WindowManager::GetSurfaceFormat().colorSpace;
			createInfo.imageExtent = WindowManager::GetExtent();
			createInfo.imageArrayLayers = 1;
			createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	

			/*
			VK_SHARING_MODE_EXCLUSIVE: An image is owned by one queue family at a time and ownership must be explicitly transferred before using it in another queue family. This option offers the best performance.
			VK_SHARING_MODE_CONCURRENT: Images can be used across multiple queue families without explicit ownership transfers.
			*/
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0; // Optional
			createInfo.pQueueFamilyIndices = nullptr; // Optional
			createInfo.preTransform = PresentingDevice->GetPhysicalDevice().presentCapabilities->capabilities.currentTransform;
			createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

			createInfo.presentMode = WindowManager::GetPresentMode();
			createInfo.clipped = VK_TRUE;


			if (vkCreateSwapchainKHR(PresentingDevice->GetVkDevice(), &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
				LOGGER.Critical("Failed to create Swapchain", true);
				return;
			}

			LOGGER.Log("Window Swapchain Created. " + std::to_string(WindowManager::GetImageCount()) + " Images. Format:" + string_VkFormat(WindowManager::GetSurfaceFormat().format) +
				" Color Space:" + string_VkColorSpaceKHR(WindowManager::GetSurfaceFormat().colorSpace) + " Present Mode:" + string_VkPresentModeKHR(WindowManager::GetPresentMode()));

			uint32_t imagecount;
			vkGetSwapchainImagesKHR(PresentingDevice->GetVkDevice(), swapChain, &imagecount, nullptr);
			Images.resize(imagecount);
			vkGetSwapchainImagesKHR(PresentingDevice->GetVkDevice(), swapChain, &imagecount, Images.data());
			ImageViews.resize(Images.size());
			for (size_t i = 0; i < Images.size(); i++) {
				VkImageViewCreateInfo createInfo{};
				createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				createInfo.image = Images[i];
				createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
				createInfo.format = WindowManager::GetSurfaceFormat().format;
				createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				createInfo.subresourceRange.baseMipLevel = 0;
				createInfo.subresourceRange.levelCount = 1;
				createInfo.subresourceRange.baseArrayLayer = 0;
				createInfo.subresourceRange.layerCount = 1;
				if (vkCreateImageView(PresentingDevice->GetVkDevice(), &createInfo, nullptr, &ImageViews[i]) != VK_SUCCESS) {
					LOGGER.Critical("Failed to create Image View", true);
				}

			}
			LOGGER.Log("Image Views Created");

			Framebuffers.resize(Images.size());
			for (int i = 0; i < ImageViews.size(); i++)
			{
				VkImageView attachments[] = {
					 ImageViews[i]
				};

				VkFramebufferCreateInfo framebufferInfo{};
				framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
				framebufferInfo.renderPass = Render::Renderer::GetRenderPass();
				framebufferInfo.attachmentCount = 1;
				framebufferInfo.pAttachments = attachments;
				framebufferInfo.width = WindowManager::GetExtent().width;
				framebufferInfo.height = WindowManager::GetExtent().height;
				framebufferInfo.layers = 1;

				if (vkCreateFramebuffer(PresentingDevice->GetVkDevice(), &framebufferInfo, nullptr, &Framebuffers[i]) != VK_SUCCESS) {
					LOGGER.Critical("Failed to create Framebuffer", true);
				}

			}
			LOGGER.Log("Framebuffers Created");

			VkFenceCreateInfo fenceInfo{};
			fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			vkCreateFence(PresentingDevice->GetVkDevice(), &fenceInfo, nullptr, &PresentingFence);

			Initialized = true;
		}

		bool Landmark::IO::Window::GetShouldClose()
		{
			return glfwWindowShouldClose(_Window);
		}

		void Landmark::IO::Window::SetShouldClose(bool state)
		{
			glfwSetWindowShouldClose(_Window, state);
		}

		void Landmark::IO::Window::PushNextFrame()
		{
	/*
			auto device = Vk::DeviceManager::GetMainPresentingDevice()->GetVkDevice();
			vkResetFences(device, 1, &PresentingFence);
			
			


			uint32_t imageIndex;
			vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, VK_NULL_HANDLE, PresentingFence, &imageIndex);


			VkPresentInfoKHR presentInfo{};
			presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

			presentInfo.waitSemaphoreCount = 0;
			VkSwapchainKHR swapChains[] = { swapChain };
			presentInfo.swapchainCount = 1;
			presentInfo.pSwapchains = swapChains;
			presentInfo.pImageIndices = &imageIndex;

			auto presentingTask = WindowManager::PresentTask;
			vkQueuePresentKHR(presentingTask->GetOwner()->GetQueue(),&presentInfo);


			vkWaitForFences(device, 1, &PresentingFence, VK_TRUE, UINT64_MAX);
			*/
		}



		void Landmark::IO::Window::MakeCurrent()
		{
			glfwMakeContextCurrent(_Window);
		}
