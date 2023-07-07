#include "WindowManager.h"
#include "../VK/VulkanEvents.h"
#include "GLFW/glfw3.h"
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

		};

		PresentTask = e.DeclareTask(request);
		});

	LOGGER.Log("Creating Main Window");
	_MainWindow = CreateWindow();
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
	for (auto it = _Windows.begin();it != _Windows.end();++it)
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


