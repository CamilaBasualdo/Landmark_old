#include <thread>
#include <iostream>
#include <vector>
#include <GLFW/glfw3.h>
#include "../src/Landmark.h"
#include "../src/Logic/RenderLogic.h"
#include <Windows.h>
#include <functional>
#include "../src/Containers/lobby.h"
#include <vector>
#include "../src/Component/Component.h"
#include "../src/Entity/Entity.h"
#ifdef _DEBUG
#define APPSTART int main(void)
#else
#define APPSTART int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
#endif

APPSTART
{
	Landmark::EngineInitParameters InitParameters;
	InitParameters.DebugMode = true;
	Landmark::Engine::Init(InitParameters);
	//auto rl = new Landmark::Render::RenderLogic();
	//delete rl;
	/*
	
	if (!glfwInit()) {
		std::cout << "Failed to Initialize GLFW!\n";
	}
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
	auto window = glfwCreateWindow(800, 600, "Vulkan", nullptr, nullptr);
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Landmark Demo";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "Landmark";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;
	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledExtensionCount = 0;
	createInfo.ppEnabledExtensionNames = nullptr;
	createInfo.enabledLayerCount = 0;

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = glfwExtensions;

	std::vector<const char*> requiredExtensions;

	for (uint32_t i = 0; i < glfwExtensionCount; i++) {
		requiredExtensions.emplace_back(glfwExtensions[i]);
	}

	requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
	std::cout << "Requested Extensions:\n";
	for (auto extension : requiredExtensions) {
		std::cout << extension << "\n";
	}
	createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

	createInfo.enabledExtensionCount = (uint32_t)requiredExtensions.size();
	createInfo.ppEnabledExtensionNames = requiredExtensions.data();
	VkInstance instance;
	VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

	if (result == VK_SUCCESS) {
		std::cout << "Vulkan Instance Created!\n";
	}
	else
		std::cout << "Vulkan Instance failed to be initialized!\n";

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

	}
	glfwDestroyWindow(window);
	*/
}