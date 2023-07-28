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
	class Event_MainSurfaceInit : Events::Event {};
	class Event_WindowFormatsSelected: Events::Event {};


	class WindowManager : Module, Events::EventSubscriber, Events::EventDispatcher
	{
		friend Window;
		
		static inline Window* _MainWindow = nullptr;

		static inline Logger LOGGER = Logger("WindowManager");


	
		void SelectFormats();
		inline static VkSurfaceFormatKHR SelectedSurfaceFormat;
		inline static VkPresentModeKHR SelectedPresentMode;
		inline static uint32_t SelectedImageCount;
		inline static VkExtent2D SelectedExtent;

	protected:
		static inline Vk::Task* PresentTask = nullptr;
	public:
		static VkSurfaceFormatKHR GetSurfaceFormat()  { return SelectedSurfaceFormat; }
		static VkPresentModeKHR GetPresentMode()  { return SelectedPresentMode; }
		static uint32_t GetImageCount()  { return SelectedImageCount; }
		static VkExtent2D GetExtent()  { return SelectedExtent; }

		const static inline std::vector<VkSurfaceFormatKHR> PreferredFormats = {
			{VK_FORMAT_B8G8R8A8_SRGB ,VK_COLOR_SPACE_SRGB_NONLINEAR_KHR}
		};

		const static inline std::vector<VkPresentModeKHR> PreferredPresentingModes = {
			VK_PRESENT_MODE_MAILBOX_KHR,
			VK_PRESENT_MODE_IMMEDIATE_KHR,
			VK_PRESENT_MODE_FIFO_RELAXED_KHR,
			VK_PRESENT_MODE_FIFO_KHR
		};
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

