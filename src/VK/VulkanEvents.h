#pragma once
#include <map>

#include "../Events/Event.h"
#include <vulkan/vulkan.hpp>
#include "Task.h"
#include "Devices/Device.h"
#include "Devices/PhysicalDevice.h"
namespace Landmark
{
	namespace Vk
	{
		class Event_VulkanInstancePreInit : Events::Event {};
		class Event_VulkanInstanceInit : Events::Event {};

		class Event_VulkanDeviceInit : Events::Event
		{
			
		};
		

		
		class Event_GpuTaskRequest : Events::Event {
		public:
			struct TaskRequest {
				const std::string Name = "Default Task Name";
				const Task::TaskIntensities _type = Task::TaskIntensities::INVALID;
				const PhysicalDevice::PhysicalDeviceID DeviceID = -1;
				uint8_t RequestedCapabilities = 0b000000;
				std::vector<std::string> Extensions;
				std::vector<std::string> Layers;
				
			};
			struct FullTaskRequest {
				TaskRequest _ShallowRequest;
				Task* _task;
			};
		private:
			
			std::map<PhysicalDevice::PhysicalDeviceID, std::vector<FullTaskRequest>> DeviceRequests;
		public:
		
			const std::vector<PhysicalDevice>& AvailableDevices;
			
			
			Task* DeclareTask(TaskRequest _request) {
				Task* newtask = new Task(_request.Name,_request._type);
				

				DeviceRequests[_request.DeviceID].push_back( FullTaskRequest{ _request,newtask });

				return newtask;
			}

			const std::map<PhysicalDevice::PhysicalDeviceID, std::vector<FullTaskRequest>> GetRequests() { return DeviceRequests; }
			
			Event_GpuTaskRequest();
		};
	}
}