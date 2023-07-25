#pragma once
#include <map>
#include <string>
#include <vulkan/vulkan.hpp>
#include <vector>
#include "Extension.h"
#include "Layer.h"
#include "../Task.h"
#include "QueueFamily.h"
namespace Landmark
{
	namespace Vk
	{
		class DeviceManager;
		class Device;

		enum PhysicalDeviceTypes
		{
			CPU = VK_PHYSICAL_DEVICE_TYPE_CPU,
			INTEGRATED = VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU,
			DISCRETE = VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU,
			VIRTUAL = VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU,
			OTHER = VK_PHYSICAL_DEVICE_TYPE_OTHER
		};
		 std::string DeviceTypes_toString(PhysicalDeviceTypes _type);
		class PhysicalDevice
		{
			friend DeviceManager;
			friend Device;
			friend QueueFamily;
		public:using PhysicalDeviceID = uint32_t;

		protected:
			PhysicalDevice(VkPhysicalDevice _pd,PhysicalDeviceID ID);

		private:
			const VkPhysicalDevice _PhysicalDevice;

			const VkPhysicalDeviceProperties VkProperties;

			const VkPhysicalDeviceRayTracingPipelinePropertiesKHR VkRayTracingProperties;
			const VkPhysicalDeviceFeatures VkFeatures;


		public:
			
			const std::string Name;
			const PhysicalDeviceTypes _DeviceType;
			const PhysicalDeviceID InternalVkDeviceID;
			const PhysicalDeviceID DeviceID;

			const std::vector<Extension> AvailableExtensions;
			const std::vector<Layer> AvailableLayers;
			const std::vector<QueueFamily> QueueFamilies;

			struct PresentCapabilities_T
			{
				VkSurfaceCapabilitiesKHR capabilities;
				std::vector<VkSurfaceFormatKHR> formats;
				std::vector<VkPresentModeKHR> presentModes;
			};

			const bool PresentCapable;
			const PresentCapabilities_T* presentCapabilities = nullptr;

		protected:
			VkPhysicalDevice GetVkDevice()const { return _PhysicalDevice; }
			
		public:

		};

	}
}
