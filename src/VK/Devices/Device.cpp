#include "Device.h"
namespace Landmark
{
	namespace Vk
	{
		namespace Devices
		{
			std::string Device::DeviceTypes_toString(DeviceTypes _type)
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

			Device::Device(VkPhysicalDevice _PhysicalDevice) :
				PhysicalDevice(_PhysicalDevice),
				Properties([this]() {VkPhysicalDeviceProperties prop; vkGetPhysicalDeviceProperties(PhysicalDevice, &prop); return prop; }()),
				Features([this]() {VkPhysicalDeviceFeatures feat; vkGetPhysicalDeviceFeatures(PhysicalDevice, &feat); return feat; }()),
				ID(Properties.deviceID),
				DeviceType(static_cast<DeviceTypes>(Properties.deviceType)),
				Name(Properties.deviceName),

				AvailableExtensions([this]() {
				uint32_t ExtensionCount;
				vkEnumerateDeviceExtensionProperties(PhysicalDevice, nullptr, &ExtensionCount, nullptr);
				std::vector<VkExtensionProperties> _Extensions(ExtensionCount);
				vkEnumerateDeviceExtensionProperties(PhysicalDevice, nullptr, &ExtensionCount, _Extensions.data());
				std::vector<Extension> ActualExtensions;
				for (auto ext : _Extensions)
					ActualExtensions.push_back(Extension(ext));
				return ActualExtensions;
					}()),

				AvailableLayer([this]()
					{
						uint32_t LayerCount;

						vkEnumerateDeviceLayerProperties(PhysicalDevice, &LayerCount, nullptr);
						std::vector<VkLayerProperties> _Layers(LayerCount);
						vkEnumerateDeviceLayerProperties(PhysicalDevice, &LayerCount, _Layers.data());
						std::vector<Layer> ActualLayers;
						for (auto lay : _Layers)
							ActualLayers.push_back(Layer(lay));
						return ActualLayers;
					}())

			{




			}
		}
	}
}

