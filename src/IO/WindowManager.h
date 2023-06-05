#pragma once
#include "../Module/Module.h"
#include "Window.h"
#include "../Events/EventSystem.h"

namespace Landmark
{
	namespace Vk
	{
		class Task;
	}
}

namespace Landmark::IO
{
	class WindowManager : Module, Events::EventSubscriber
	{

		static inline Vk::Task* PresentTask = nullptr;
	public:
		WindowManager();
		std::string GetName() const override { return "WindowManager"; }
		void PreInit() override;
		void Init() override;
		void PostInit() override;

		static inline std::list<Window> _Windows;
		static Window* CreateWindow();
		static const std::list<Window>& GetWindows() { return _Windows; }
	};
}

