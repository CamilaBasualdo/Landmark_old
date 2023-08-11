#pragma once
#include <Module/Module.h>
#include <Events/EventSystem.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

#include "VK/Task.h"

namespace Landmark::Debug
{
	class ImguiModule : Landmark::Module,Events::EventSubscriber
	{

		static inline ImGui_ImplVulkanH_Window IMGUI_WINDOW;

		static inline Vk::Task* Imgui_RenderingTask;
	public:
		void PreInit() override;
		void PostInit() override;
	};

}
