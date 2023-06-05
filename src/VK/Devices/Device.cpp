#include "Device.h"
#include "../../IO/WindowManager.h"
namespace Landmark
{
	namespace Vk
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

		const std::vector<Extension> Device::GetExtensions()
		{
			uint32_t ExtensionCount;
			vkEnumerateDeviceExtensionProperties(PhysicalDevice, nullptr, &ExtensionCount, nullptr);
			std::vector<VkExtensionProperties> _Extensions(ExtensionCount);
			vkEnumerateDeviceExtensionProperties(PhysicalDevice, nullptr, &ExtensionCount, _Extensions.data());
			std::vector<Extension> ActualExtensions;
			for (auto ext : _Extensions)
				ActualExtensions.push_back(Extension(ext));
			return ActualExtensions;
		}

		const std::vector<Layer> Device::GetLayers()
		{
			uint32_t LayerCount;

			vkEnumerateDeviceLayerProperties(PhysicalDevice, &LayerCount, nullptr);
			std::vector<VkLayerProperties> _Layers(LayerCount);
			vkEnumerateDeviceLayerProperties(PhysicalDevice, &LayerCount, _Layers.data());
			std::vector<Layer> ActualLayers;
			for (auto lay : _Layers)
				ActualLayers.push_back(Layer(lay));
			return ActualLayers;
		}

		const std::map<Task::CapabilitiesMask, int> Device::GetQueueCapacities()
		{
			std::map<Task::CapabilitiesMask, int> _Capabilities;

			uint32_t queueFamilyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevice, &queueFamilyCount, nullptr);

			std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevice, &queueFamilyCount, queueFamilies.data());


			for (int i =0 ; i < queueFamilyCount;i++)
			{
				auto family = queueFamilies[i];
				Task::CapabilitiesMask Mask = 0b0;
				if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT) Mask |= Task::GRAPHICS;
				if (family.queueFlags & VK_QUEUE_COMPUTE_BIT) Mask |= Task::COMPUTE;
				if (family.queueFlags & VK_QUEUE_TRANSFER_BIT) Mask |= Task::TRANSFER;
				if (family.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) Mask |= Task::SPARSE_BINDING;

				VkBool32 PresentSupport = VK_FALSE;
				auto Windows= IO::WindowManager::GetWindows();
				for (auto window : Windows)
				{
					vkGetPhysicalDeviceSurfaceSupportKHR(PhysicalDevice, i, window.GetSurface(), &PresentSupport);
					if (PresentSupport) break;
				}
				if (PresentSupport == VK_TRUE) Mask |= Task::PRESENT;

				_Capabilities[Mask] = family.queueCount * 100;
				
			}
			return _Capabilities;
		}

		Device::Device(VkPhysicalDevice _PhysicalDevice) :
			PhysicalDevice(_PhysicalDevice),
			Properties([this]() {VkPhysicalDeviceProperties prop; vkGetPhysicalDeviceProperties(PhysicalDevice, &prop); return prop; }()),
			Features([this]() {VkPhysicalDeviceFeatures feat; vkGetPhysicalDeviceFeatures(PhysicalDevice, &feat); return feat; }()),
			ID(Properties.deviceID),
			DeviceType(static_cast<DeviceTypes>(Properties.deviceType)),
			Name(Properties.deviceName),

			AvailableExtensions([this]() {return GetExtensions(); }()),
			AvailableLayer([this]() {return GetLayers(); }()),
			QueueCapability([this](){return GetQueueCapacities();}())

		{




		}

	}
}

