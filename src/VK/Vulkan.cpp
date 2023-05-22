#include "Vulkan.h"
#include <GLFW/glfw3.h>
#include "DeviceManager.h"
namespace Landmark
{
	namespace Vk
	{
		
		VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
			auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
			if (func != nullptr) {
				return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
			}
			else {
				return VK_ERROR_EXTENSION_NOT_PRESENT;
			}
		}
		
		void VulkanValidationLogFormat(const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData) {
			static Debug::Logger _logger = Debug::Debugger::GetLogger("Vulkan Validation");

			
		}

		static VKAPI_ATTR VkBool32 VKAPI_CALL VulkankValidationCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,VkDebugUtilsMessageTypeFlagsEXT messageType,const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,void* pUserData) {
			static Debug::Logger _logger = Debug::Debugger::GetLogger("Vulkan Validation");

			VulkanValidationLogFormat(pCallbackData);

			switch (messageSeverity) {
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
				_logger.Log(pCallbackData->pMessage);
				break;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
				_logger.Log(pCallbackData->pMessage);
				break;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
				_logger.Warning(pCallbackData->pMessage);
				break;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
				_logger.Error(pCallbackData->pMessage);
				break;

			}
			//std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

			return VK_FALSE;
		}
		 
		
		
		void Vulkan::Init(const VulkanInitParameters param) {
			
			auto EventReturn = DispatchEvent<Event_VulkanInstancePreInit>();
			InstanceInit(param);

			DeviceManager::Init();

			
		}
		
		
		void Vulkan::InstanceInit(const VulkanInitParameters param)
		{
			LOGGER.Debug("Initializing GLFW");
			if (!glfwInit()) {
				const char* Desc;
				int errorCode = glfwGetError(&Desc);
				LOGGER.Critical("GLFW Failed to initialize. Error:\n" + std::string(Desc)
					, true);

			}
			else
				LOGGER.Log("GLFW Initialized!", LOGGER.green);


			LOGGER.Debug("Initializing Vulkan");
			VkApplicationInfo appInfo{};
			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appInfo.pApplicationName = param.AppName;
			appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.pEngineName = "Landmark Engine";
			appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.apiVersion = VK_API_VERSION_1_0;


			VkInstanceCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			createInfo.pApplicationInfo = &appInfo;


			std::vector<const char*> RequestedExtensions = {};

			uint32_t glfwExtensionCount = 0;
			const char** glfwExtensions;

			glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

			for (int i = 0; i < glfwExtensionCount; i++)
				RequestedExtensions.emplace_back(glfwExtensions[i]);


			RequestedExtensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
			RequestedExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
			createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

			createInfo.enabledExtensionCount = RequestedExtensions.size();
			createInfo.ppEnabledExtensionNames = RequestedExtensions.data();

			std::string ExtensionsString = "Requested Vulkan Extensions: \n\n";

			for (int i = 0; i < createInfo.enabledExtensionCount; i++) {
				ExtensionsString += std::string(" | ") + createInfo.ppEnabledExtensionNames[i] + std::string("\n");
			}
			LOGGER.Log(ExtensionsString);

			std::vector<const char*> ValidationLayers = {};
			if (param.ValidationMode) {
				ValidationLayers.emplace_back("VK_LAYER_KHRONOS_validation");
				createInfo.enabledLayerCount = ValidationLayers.size();
				createInfo.ppEnabledLayerNames = ValidationLayers.data();

				std::string LayerText = "Requested Vulkan Layers: \n\n";
				for (auto layer : ValidationLayers)
					LayerText += " | " + std::string(layer) + "\n";
				LOGGER.Log(LayerText);
			}
			else createInfo.enabledLayerCount = 0;




			VkResult InstResult = vkCreateInstance(&createInfo, nullptr, &_VkInstance);
			if (InstResult == VK_SUCCESS)
				LOGGER.Log("Vulkan Initialized!", LOGGER.green);
			else {
				LOGGER.Critical("Vulkan Failed to Initialize!", false);
				uint32_t extensionCount = 0;
				vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
				std::vector<VkExtensionProperties> extensions(extensionCount);
				vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

				std::string AvailableEXT = "Available Extensions:\n\n";
				for (const auto& extension : extensions) {
					AvailableEXT += " | " + std::string(extension.extensionName) + '\n';
				}
				LOGGER.Critical(AvailableEXT, true);
			}

			{
				VkDebugUtilsMessengerCreateInfoEXT createInfo{};
				createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
				createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
				createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
				createInfo.pfnUserCallback = VulkankValidationCallback;
				createInfo.pUserData = nullptr; // Optional

				if (CreateDebugUtilsMessengerEXT(_VkInstance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
					LOGGER.Critical("Failed to setup Debug Messenger!");
				}
				else LOGGER.Log("Debug Messenger Created!");
			}


		}

	

}
}