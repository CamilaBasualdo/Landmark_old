#include "WindowManager.h"

Landmark::IO::WindowManager::WindowManager()
{

}

void Landmark::IO::WindowManager::PreInit()
{
}

void Landmark::IO::WindowManager::Init()
{
	
	

}

void Landmark::IO::WindowManager::PostInit()
{
}

Landmark::IO::Window* Landmark::IO::WindowManager::CreateWindow()
{
	return new Window();
}
