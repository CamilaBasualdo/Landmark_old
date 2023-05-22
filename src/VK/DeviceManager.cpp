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

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(Vulkan::GetVkInstance(), &deviceCount, devices.data());
	std::string Log = "GPUs Found:\n\n";
	for (auto device : devices) {
		VkPhysicalDeviceProperties deviceProperties;
		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceProperties(device, &deviceProperties);
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
		Log += " | " + std::string(deviceProperties.deviceName) + "\n";
	}
	LOGGER.Log(Log);

	auto e = DispatchEvent<Event_VulkanPhysicalDevicesPreInit>();

}
