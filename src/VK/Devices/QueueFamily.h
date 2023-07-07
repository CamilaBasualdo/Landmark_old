#pragma once
#include <vulkan/vulkan.hpp>
#include "../../Logger.h"
namespace Landmark::Vk
{
	class PhysicalDevice;
	enum QueueFamilyCapabilities
	{
		PRESENT = 0b1,
		GRAPHICS = 0b10,
		COMPUTE = 0b100,
		TRANSFER = 0b1000,
		SPARSE_BINDING = 0b10000
	};
	class QueueFamily
	{
	public:
		using QueueFamilyCapabilitiesMask = uint8_t;
	private:

		friend PhysicalDevice;
		

		static inline Logger LOGGER = Logger("QueueFamilies");

		QueueFamily(PhysicalDevice const* _owner, const int ID);
	public:
		const  PhysicalDevice* Owner;
		const int ID;
		const VkQueueFamilyProperties VkProperties;
		const int QueueCount;
		const QueueFamilyCapabilitiesMask Capabilities;


	public:

	};

}
