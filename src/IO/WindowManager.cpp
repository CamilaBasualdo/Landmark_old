#include "WindowManager.h"
#include "../VK/VulkanEvents.h"
Landmark::IO::WindowManager::WindowManager()
{

}

void Landmark::IO::WindowManager::PreInit()
{
	SubscribeTo<Vk::Event_GpuTaskRequest>([&](Vk::Event_GpuTaskRequest& e) {
		//LOGGER.Log("Requesting Main Rendering Task on Device 0 (HARDCODED) Type: " + std::string(string_VkPhysicalDeviceType(e.AvailableDevices[0].deviceProperties.deviceType)));


		Vk::Event_GpuTaskRequest::TaskRequest request = {
			"Presenting Task",
			Vk::Task::TaskIntensities::MEDIUM,
			e.AvailableDevices[0].ID,
			Vk::Task::PRESENT,

		};

		PresentTask = e.DeclareTask(request);
		});
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
