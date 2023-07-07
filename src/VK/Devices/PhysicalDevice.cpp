#include "PhysicalDevice.h"

Landmark::Vk::PhysicalDevice::PhysicalDevice(VkPhysicalDevice _pd, PhysicalDeviceID ID):
_PhysicalDevice(_pd),
VkProperties([this]()
	{
		VkPhysicalDeviceProperties prop;
		vkGetPhysicalDeviceProperties(_PhysicalDevice, &prop);
		return prop;
	}()),
VkFeatures([this](){VkPhysicalDeviceFeatures feat;
	vkGetPhysicalDeviceFeatures(_PhysicalDevice, &feat);
	return feat; }()),
Name(VkProperties.deviceName),
_DeviceType(static_cast<PhysicalDeviceTypes>(VkProperties.deviceType)),
AvailableExtensions([this](){uint32_t LayerCount;
	uint32_t ExtensionCount;
	vkEnumerateDeviceExtensionProperties(_PhysicalDevice, nullptr, &ExtensionCount, nullptr);
	std::vector<VkExtensionProperties> _Extensions(ExtensionCount);
	vkEnumerateDeviceExtensionProperties(_PhysicalDevice, nullptr, &ExtensionCount, _Extensions.data());
	std::vector<Extension> ActualExtensions;
	for (auto ext : _Extensions)
		ActualExtensions.push_back(Extension(ext));
	return ActualExtensions;
}()),
AvailableLayers([this](){uint32_t LayerCount;

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

}()), PresentCapable([this]()
{
		for (auto Queue : QueueFamilies)
			if (Queue.Capabilities & PRESENT) return true;
		return false;
}()),
DeviceID(ID),
InternalVkDeviceID(VkProperties.deviceID)

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
