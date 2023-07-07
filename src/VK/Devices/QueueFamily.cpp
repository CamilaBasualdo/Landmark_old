#include "QueueFamily.h"

#include "PhysicalDevice.h"
#include "../Vulkan.h"
#include "../../IO/WindowManager.h"
Landmark::Vk::QueueFamily::QueueFamily( PhysicalDevice const* _owner, const int _ID):
Owner(_owner),
ID(_ID),
VkProperties([this]()
{

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(Owner->GetVkDevice(), &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(Owner->GetVkDevice(), &queueFamilyCount, queueFamilies.data());
		return queueFamilies[ID];
}()),
	QueueCount(VkProperties.queueCount),
	Capabilities([this]()
	{
			QueueFamilyCapabilitiesMask mask = 0b0;
			if (VkProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT) mask |= QueueFamilyCapabilities::GRAPHICS;
			if (VkProperties.queueFlags & VK_QUEUE_COMPUTE_BIT) mask |= QueueFamilyCapabilities::COMPUTE;
			if (VkProperties.queueFlags & VK_QUEUE_TRANSFER_BIT) mask |= QueueFamilyCapabilities::TRANSFER;
			if (VkProperties.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) mask |= QueueFamilyCapabilities::SPARSE_BINDING;
			VkBool32 PresentSupport;
			vkGetPhysicalDeviceSurfaceSupportKHR(Owner->GetVkDevice(), ID, IO::WindowManager::MainWindow().GetSurface(), &PresentSupport);
			mask |= QueueFamilyCapabilities::PRESENT * PresentSupport;

			return mask;
		
	}())
{
}
