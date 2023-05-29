#pragma once
#include <vulkan/vulkan.h>
#include <string>
class Extension
{

	const VkExtensionProperties Properties;
	const std::string Name;

	const uint32_t Version;

public:
	Extension(VkExtensionProperties _prop);
};
