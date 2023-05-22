#include "RenderLogic.h"

namespace Landmark
{
	namespace Render
	{
		RenderLogic::RenderLogic()
		{
			
			
		}
	
		void RenderLogic::PreInit() 
		{
			SubscribeTo<Vk::Event_VulkanQueuesPreInit>([&](Vk::Event_VulkanQueuesPreInit& e) {

				});
			SubscribeTo<Vk::Event_VulkanPhysicalDevicesPreInit>([&](Vk::Event_VulkanPhysicalDevicesPreInit& e) {

				});
		}
		void RenderLogic::Init() 		
		{
		}
		void RenderLogic::PostInit() 
		{
		}
	}
}
