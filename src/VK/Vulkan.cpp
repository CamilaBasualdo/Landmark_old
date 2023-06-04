#include "Vulkan.h"

#include <GLFW/glfw3.h>
#include "Devices/DeviceManager.h"
#include <sstream>
#include <vulkan/vk_enum_string_helper.h>
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
		
		std::string VulkanValidationLogFormat(const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData) {
			//static Debug::Logger _logger = Debug::Debugger::GetLogger("Vulkan Validation");

			std::string ValidationLog = "\n=== " + std::string(pCallbackData->pMessageIdName) +" ===\n";
			ValidationLog += std::to_string(pCallbackData->objectCount) +" Object(s) \n";
			for (int i = 0; i < pCallbackData->objectCount; i++) {
				ValidationLog += " " + std::to_string(i) + "| ";
				ValidationLog += (pCallbackData->pObjects[i].pObjectName) ? pCallbackData->pObjects[i].pObjectName : "No Name" + std::string(" | ");
				
				ValidationLog += std::string(string_VkObjectType(pCallbackData->pObjects[i].objectType)) + " | ";

				std::ostringstream oss; oss<< "0x" << reinterpret_cast<void*>(pCallbackData->pObjects[i].objectHandle);
				ValidationLog += oss.str() + "\n";
				
			}

			//QUEUES
			ValidationLog += std::to_string(pCallbackData->queueLabelCount) + " Queue(s) \n";

			for (int i = 0; i < pCallbackData->queueLabelCount; i++) {
				ValidationLog += " " + std::to_string(i) + "| ";
				ValidationLog += ((pCallbackData->pQueueLabels->pLabelName) ? pCallbackData->pQueueLabels->pLabelName : "No Name")  + std::string("\n");
			
				//std::ostringstream oss; oss << reinterpret_cast<void*>(pCallbackData->pQueueLabels->);
				//ValidationLog += oss.str() + "\n";

			}
			ValidationLog += std::to_string(pCallbackData->cmdBufLabelCount) + " CmdBuffer(s) \n";
			if (pCallbackData->cmdBufLabelCount == 0) ValidationLog += "None\n";
			else ValidationLog += "\n";
			for (int i = 0; i < pCallbackData->cmdBufLabelCount; i++) {
				ValidationLog += " " + std::to_string(i) + "| ";
				ValidationLog += ((pCallbackData->pCmdBufLabels->pLabelName) ? pCallbackData->pCmdBufLabels->pLabelName : "No Name") + std::string("\n");

				//std::ostringstream oss; oss << reinterpret_cast<void*>(pCallbackData->pQueueLabels->);
				//ValidationLog += oss.str() + "\n";

			}
			ValidationLog += std::string("Log:\n") + pCallbackData->pMessage + std::string("\n");

			ValidationLog += "========================================================\n";
			return ValidationLog;
			//_logger.Log(ValidationLog);

			
		}

		static VKAPI_ATTR VkBool32 VKAPI_CALL VulkankValidationCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,VkDebugUtilsMessageTypeFlagsEXT messageType,const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,void* pUserData) {
			static Logger _logger = Logger("Vulkan Validation");

			auto ss = VulkanValidationLogFormat(pCallbackData);
			
			
			switch (messageSeverity) {
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
				_logger.Log(ss);
				break;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
				_logger.Log(ss);
				break;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
				_logger.Warning(ss);
				break;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
				_logger.Error(ss);
				break;

			}
			
			//std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

			return VK_FALSE;
		}
		 
		
		
	

		std::string Vulkan::GetName() const
		{
			return std::string();
		}

		void Vulkan::PreInit()
		{
		}

		void Vulkan::Init()
		{
			
			InstanceInit();

			DeviceManager::Init();

		
		}

		void Vulkan::PostInit()
		{
		}
		
		
		void Vulkan::InstanceInit()
		{
			auto EventReturn = Dispatch<Event_VulkanInstancePreInit>();
			LOGGER.Debug("Initializing GLFW");
			if (!glfwInit()) {
				const char* Desc;
				int errorCode = glfwGetError(&Desc);
				LOGGER.Critical("GLFW Failed to initialize. Error:\n" + std::string(Desc)
					, true);

			}
			else
				LOGGER.Log("GLFW Init");


			LOGGER.Debug("Initializing Vulkan");
			VkApplicationInfo appInfo{};
			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appInfo.pApplicationName = "";
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
#ifdef _DEBUG
			ValidationLayers.emplace_back("VK_LAYER_KHRONOS_validation");
			createInfo.enabledLayerCount = ValidationLayers.size();
			createInfo.ppEnabledLayerNames = ValidationLayers.data();

			std::string LayerText = "Requested Vulkan Layers: \n\n";
			for (auto layer : ValidationLayers)
				LayerText += " | " + std::string(layer) + "\n";
			LOGGER.Log(LayerText);

#elif
			createInfo.enabledLayerCount = 0;

#endif
			




			VkResult InstResult = vkCreateInstance(&createInfo, nullptr, &_VkInstance);
			if (InstResult == VK_SUCCESS)
				LOGGER.Log("Vulkan Init");
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
				createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT| VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
				createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT ;
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