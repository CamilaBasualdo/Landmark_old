#include "PhysicalDevice.h"

#include "../../IO/WindowManager.h"
Landmark::Vk::PhysicalDevice::PhysicalDevice(VkPhysicalDevice _pd, PhysicalDeviceID ID):
_PhysicalDevice(_pd),
VkProperties([this]()
{
	VkPhysicalDeviceProperties prop;
	vkGetPhysicalDeviceProperties(_PhysicalDevice, &prop);
	return prop;
}()),
VkRayTracingProperties([this]()
{
	VkPhysicalDeviceRayTracingPipelinePropertiesKHR prop{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_PROPERTIES_KHR };

	VkPhysicalDeviceProperties2 acProp{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2 };
	acProp.pNext = &prop;
	vkGetPhysicalDeviceProperties2(_PhysicalDevice, &acProp);

	return prop;
}()),


VkFeatures([this](){VkPhysicalDeviceFeatures feat;
	vkGetPhysicalDeviceFeatures(_PhysicalDevice, &feat);
	return feat; }()),

Name(VkProperties.deviceName),
_DeviceType(static_cast<PhysicalDeviceTypes>(VkProperties.deviceType)),
InternalVkDeviceID(VkProperties.deviceID),
DeviceID(ID),
AvailableExtensions([this](){uint32_t LayerCount;
		uint32_t ExtensionCount;
		vkEnumerateDeviceExtensionProperties(_PhysicalDevice, nullptr, &ExtensionCount, nullptr);
		std::vector<VkExtensionProperties> _Extensions(ExtensionCount);
		vkEnumerateDeviceExtensionProperties(_PhysicalDevice, nullptr, &ExtensionCount, _Extensions.data());
		std::vector<Extension> ActualExtensions;
		for (auto ext : _Extensions)
			ActualExtensions.push_back(Extension(ext));
		return ActualExtensions;
	}()), AvailableLayers([this](){uint32_t LayerCount;

	vkEnumerateDeviceLayerProperties(_PhysicalDevice, &LayerCount, nullptr);
	std::vector<VkLayerProperties> _Layers(LayerCount);
	vkEnumerateDeviceLayerProperties(_PhysicalDevice, &LayerCount, _Layers.data());
	std::vector<Layer> ActualLayers;
	for (auto lay : _Layers)
		ActualLayers.push_back(Layer(lay));
	return ActualLayers; }()),
QueueFamilies([this]()
{
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(_PhysicalDevice, &queueFamilyCount, nullptr);

	std::vector<QueueFamily> FinalFamilies;
	FinalFamilies.reserve(queueFamilyCount);

	for (int i = 0 ; i < queueFamilyCount;i++)
	{
		FinalFamilies.push_back(QueueFamily(this,i));
	}
	return FinalFamilies;

}())
,
PresentCapable([this]()
{
	for (auto Queue : QueueFamilies)
		if (Queue.Capabilities & PRESENT) return true;
	return false;
}()),
presentCapabilities([this]()
{
	if (!PresentCapable) return (PresentCapabilities_T*)nullptr;
	PresentCapabilities_T* pc = new PresentCapabilities_T();
	auto surface = IO::WindowManager::MainWindow().GetSurface();
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_PhysicalDevice, surface, &pc->capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(_PhysicalDevice, surface, &formatCount, nullptr);

	if (formatCount != 0) {
		pc->formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(_PhysicalDevice, surface, &formatCount, pc->formats.data());
	}
	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(_PhysicalDevice, surface, &presentModeCount, nullptr);

	if (presentModeCount != 0) {
		pc->presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(_PhysicalDevice, surface, &presentModeCount, pc->presentModes.data());
	}
	return pc;

}())

{
	
}

std::string Landmark::Vk::DeviceTypes_toString(PhysicalDeviceTypes _type)
{
	switch (_type) {
	case CPU: return "Cpu";
	case INTEGRATED: return "Integrated";
	case DISCRETE: return "Discrete";
	case VIRTUAL: return "Virtual";
	case OTHER: return "Other";
	default:;
	}
}
