#pragma once
#include <string>
#include <vulkan/vulkan.hpp>
namespace Landmark
{
	namespace Vk
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


