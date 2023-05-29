#pragma once
#include <string>
#include <vulkan/vulkan.h>
namespace Landmark
{
	namespace Vk
	{
		namespace Devices
		{
			class Layer
			{
				const VkLayerProperties Properties;
				const std::string Name;
				const std::string Description;
				const uint32_t Version;
				const uint32_t ImplementationVersion;
			public:
				Layer(VkLayerProperties _prop);
			};
		}
		
	}
}


