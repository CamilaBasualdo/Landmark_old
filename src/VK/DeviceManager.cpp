#include "DeviceManager.h"
#include <vulkan/vulkan.h>
#include "VulkanEvents.h"
#include "Vulkan.h"
void Landmark::Vk::DeviceManager::Init()
{


	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(Vulkan::GetVkInstance(), &deviceCount, nullptr);

	if (deviceCount == 0) {
		LOGGER.Critical("No GPUs With Vulkan support found!", true);
	}
	std::vector<std::string> gpus = {};
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(Vulkan::GetVkInstance(), &deviceCount, devices.data());
	std::string Log = "GPUs Found:\n\n";
	for (auto device : devices) {
		VkPhysicalDeviceProperties deviceProperties;
		VkPhysicalDeviceFeatures deviceFeatures;

		vkGetPhysicalDeviceProperties(device, &deviceProperties);
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
		gpus.push_back(deviceProperties.deviceName);
	}


	LOGGER.Log_List("GPUs Found:", gpus);
	std::vector<Event_VulkanPhysicalDevicesPreInit::AvailableDeviceDetails> _DevicesForEvent;

	for (auto device : devices) {
		//PROPERTIES
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(device, &deviceProperties);

		//FEATURES
		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

		//EXTENSIONS
		uint32_t ExtensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &ExtensionCount, nullptr);
		std::vector<VkExtensionProperties> deviceExtensions(ExtensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &ExtensionCount, deviceExtensions.data());
		
		//LAYERS
		uint32_t LayerCount;
		vkEnumerateDeviceLayerProperties(device, &LayerCount, nullptr);
		std::vector<VkLayerProperties> DeviceLayers(LayerCount);
		vkEnumerateDeviceLayerProperties(device, &LayerCount, DeviceLayers.data());
		
		
		
		_DevicesForEvent.push_back({ deviceProperties,deviceFeatures,deviceExtensions,DeviceLayers });
	}
	auto e = DispatchEvent<Event_VulkanPhysicalDevicesPreInit>(_DevicesForEvent);

}
