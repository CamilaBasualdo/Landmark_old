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
		static inline Window* _MainWindow = nullptr;

		static inline Logger LOGGER = Logger("WindowManager");

	public:
		WindowManager();
		std::string GetName() const override { return "WindowManager"; }
		void PreInit() override;
		void Init() override;
		void PostInit() override;

		static inline std::list<Window> _Windows;
		static Window* CreateWindow();
		static void DestroyWindow(Window* _window);
		static const std::list<Window>& GetWindows() { return _Windows; }

		static Window& MainWindow() { return *_MainWindow; }

		
		void Update() override;
	};
}

