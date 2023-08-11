#pragma once
#include <map>

#include "../Events/Event.h"
#include <vulkan/vulkan.hpp>
#include "Task.h"
#include "Devices/Device.h"
#include "Devices/PhysicalDevice.h"
namespace Landmark
{
	namespace Events
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
				const Vk::Task::TaskIntensities _type = Vk::Task::TaskIntensities::INVALID;
				const Vk::PhysicalDevice::PhysicalDeviceID DeviceID = -1;
				uint8_t RequestedCapabilities = 0b000000;
				std::vector<std::string> Extensions;
				std::vector<std::string> Layers;
				
			};
			struct FullTaskRequest {
				TaskRequest _ShallowRequest;
				Vk::Task* _task;
			};
		private:
			
			std::map<Vk::PhysicalDevice::PhysicalDeviceID, std::vector<FullTaskRequest>> DeviceRequests;
		public:
		
			const std::vector<Vk::PhysicalDevice>& AvailableDevices;
			
			
			Vk::Task* DeclareTask(TaskRequest _request) {
				Vk::Task* newtask = new Vk::Task(_request.Name,_request._type);
				

				DeviceRequests[_request.DeviceID].push_back( FullTaskRequest{ _request,newtask });

				return newtask;
			}

			const std::map<Vk::PhysicalDevice::PhysicalDeviceID, std::vector<FullTaskRequest>> GetRequests() { return DeviceRequests; }
			
			Event_GpuTaskRequest();
		};
	}
}