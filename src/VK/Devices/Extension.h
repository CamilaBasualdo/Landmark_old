#pragma once
#include <vulkan/vulkan.hpp>
#include <string>
namespace Landmark::Vk
{
	class Extension
	{

		const VkExtensionProperties Properties;
		const std::string Name;

		const uint32_t Version;

	public:
		Extension(VkExtensionProperties _prop);
	};

}
