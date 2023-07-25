#include "WindowManager.h"
#include "../VK/VulkanEvents.h"
#include "GLFW/glfw3.h"
#include "../Renderer/Renderer.h"


Landmark::IO::WindowManager::WindowManager()
{

}

void Landmark::IO::WindowManager::PreInit()
{
	SubscribeTo<Vk::Event_GpuTaskRequest>([&](Vk::Event_GpuTaskRequest& e) {
		//LOGGER.Log("Requesting Main Rendering Task on Device 0 (HARDCODED) Type: " + std::string(string_VkPhysicalDeviceType(e.AvailableDevices[0].deviceProperties.deviceType)));


		Vk::Event_GpuTaskRequest::TaskRequest request = {
			"Window Presenting Task",
			Vk::Task::TaskIntensities::MEDIUM,
			e.AvailableDevices[0].DeviceID,
			Vk::PRESENT,
			{VK_KHR_SWAPCHAIN_EXTENSION_NAME}

		};

		PresentTask = e.DeclareTask(request);
		});
	SubscribeTo<Render::Event_RendererInit>([&](Render::Event_RendererInit& e)
		{
			LOGGER.Log("Renderer Finished setting up Renderpass. Finalizing Windows");
			_MainWindow->Init();
			
			for (auto& Window : _Windows)
				Window.Init();



		});

	SubscribeTo<Vk::Event_VulkanInstanceInit>([&](Vk::Event_VulkanInstanceInit& e)
		{
			LOGGER.Log("Vulkan Instance Finished. Creating Main Surface & Window");
			_MainWindow = CreateWindow();
			
			Dispatch<Event_MainSurfaceInit>();
		});
	
	SubscribeTo<Vk::Event_VulkanDeviceInit>([&](Vk::Event_VulkanDeviceInit& e)
	{
			LOGGER.Log("Device Manager Finished. Chooseing Formats");
			SelectFormats();
			LOGGER.Log("Formats Selected");
			Dispatch<Event_WindowFormatsSelected>();
	});

	
}
void Landmark::IO::WindowManager::SelectFormats()
{
	auto PresentingDevice = Vk::DeviceManager::GetMainPresentingDevice();
	if (!PresentingDevice)
	{
		LOGGER.Critical("System Not capable of presenting!", true);
	}
	auto& PresentCapabilities = PresentingDevice->GetPhysicalDevice().presentCapabilities;

	if (PresentCapabilities->formats.empty() || PresentCapabilities->presentModes.empty())
		LOGGER.Critical("System Has no Available presenting formats", true);
	auto& AvailableFormats = PresentCapabilities->formats;

	bool FormatFound = false;
	for (auto& f : WindowManager::PreferredFormats)
	{
		for (auto& avF : AvailableFormats)
		{
			if (avF.colorSpace == f.colorSpace && avF.format == f.format)
			{
				SelectedSurfaceFormat = avF;
				FormatFound = true;
				break;
			}
		}

	}
	if (!FormatFound)
		LOGGER.Critical("No available Surface format found from preferred", true);
	auto& AvailablePresentModes = PresentCapabilities->presentModes;

	bool PresentModeFound = false;
	for (auto& f : WindowManager::PreferredPresentingModes)
	{
		if (std::find(AvailablePresentModes.begin(), AvailablePresentModes.end(), f) != AvailablePresentModes.end())
		{
			SelectedPresentMode = f;
			PresentModeFound = true;
			break;
		}
	}
	if (!PresentModeFound)
		LOGGER.Critical("No available Present mode found from preferred", true);

	int width, height;
	glfwGetFramebufferSize(MainWindow()._Window, &width, &height);
	SelectedExtent = {
		  static_cast<uint32_t>(width),
		  static_cast<uint32_t>(height)
	};

	auto presentRawCapabilities = PresentCapabilities->capabilities;
	SelectedExtent.width = std::clamp(SelectedExtent.width, presentRawCapabilities.minImageExtent.width, presentRawCapabilities.maxImageExtent.width);
	SelectedExtent.height = std::clamp(SelectedExtent.height, presentRawCapabilities.minImageExtent.height, presentRawCapabilities.maxImageExtent.height);

	/*
	 However, simply sticking to this minimum means that we may sometimes have to wait on the driver to complete
	 internal operations before we can acquire another image to render to. Therefore it is recommended to request
	 at least one more image than the minimum:
	 **/
	SelectedImageCount = presentRawCapabilities.minImageCount + 1;
	SelectedImageCount = std::min(presentRawCapabilities.maxImageCount, SelectedImageCount);




	/*
	VK_SHARING_MODE_EXCLUSIVE: An image is owned by one queue family at a time and ownership must be explicitly transferred before using it in another queue family. This option offers the best performance.
	VK_SHARING_MODE_CONCURRENT: Images can be used across multiple queue families without explicit ownership transfers.
	*/

}

void Landmark::IO::WindowManager::Init()
{
	
}





void Landmark::IO::WindowManager::PostInit()
{
}

Landmark::IO::Window* Landmark::IO::WindowManager::CreateWindow()
{
	_Windows.push_back(Window());
	auto NewWindow = &_Windows.back();
	return NewWindow;
}

void Landmark::IO::WindowManager::DestroyWindow(Window* _window)
{
	if (_window == _MainWindow)
	{
		LOGGER.Error("Call to destroy window cannot destroy Main Window!");
		return;
	}
	for (auto it = _Windows.begin(); it != _Windows.end(); ++it)
	{
		if (&(*it) == _window)
		{
			_Windows.erase(it);
			return;
		}
	}
}

void Landmark::IO::WindowManager::Update()
{
	glfwPollEvents();
}


