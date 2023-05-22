#pragma once
#include "../Events/Event.h"
#include <vulkan/vulkan.h>
namespace Landmark
{
	namespace Vk
	{
		class Event_VulkanInstancePreInit : Events::Event {
		public:

		};

		class Event_VulkanQueuesPreInit : Events::Event
		{
		public:
			void RequestQueue(VkQueue* queue);
		};



		class Event_VulkanPhysicalDevicesPreInit : Events::Event {
		public:

		};
	}
}