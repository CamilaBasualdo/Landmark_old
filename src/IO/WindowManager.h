#pragma once
#include "../Module/Module.h"
#include "Window.h"	
namespace Landmark::IO
{
	class WindowManager : Module
	{
	public:
		WindowManager();
		std::string GetName() const override { return "WindowManager"; }
		void PreInit() override;
		void Init() override;
		void PostInit() override;


		static Window* CreateWindow();
	};
}

