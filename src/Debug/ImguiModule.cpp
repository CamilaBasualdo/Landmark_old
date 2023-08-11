#include "ImguiModule.h"
#include <IO/WindowManager.h>

#include "VK/Vulkan.h"

void Landmark::Debug::ImguiModule::PreInit()
{
	SubscribeTo<Events::Event_GpuTaskRequest>([&](Events::Event_GpuTaskRequest& e)
		{
			Events::Event_GpuTaskRequest::TaskRequest task{
				"Imgui Task",
				Vk::Task::LOW,
				e.AvailableDevices[0].DeviceID,
				Vk::GRAPHICS | Vk::PRESENT,
			};
			
		    Imgui_RenderingTask = e.DeclareTask(task);
		});
}

void Landmark::Debug::ImguiModule::PostInit()
{
	auto mainWindow = IO::WindowManager::MainWindow();
	auto mainWindowSize = mainWindow.GetWindowSize();
	IMGUI_WINDOW.Surface = mainWindow.GetSurface();
	IMGUI_WINDOW.SurfaceFormat = IO::WindowManager::GetSurfaceFormat();
	IMGUI_WINDOW.PresentMode = IO::WindowManager::GetPresentMode();
	auto presentingDevice = Vk::DeviceManager::GetMainPresentingDevice();
	ImGui_ImplVulkanH_CreateOrResizeWindow(Vk::Vulkan::GetVkInstance(),
		presentingDevice->GetPhysicalDevice().GetVkDevice(),
		presentingDevice->GetVkDevice(),
		&IMGUI_WINDOW, Imgui_RenderingTask->GetOwner()->GetFamilyID(),
		nullptr,
		mainWindowSize.x, mainWindowSize.y,
		2

	);
	
}
