#pragma once
#include <map>

#include "../Events/Event.h"
#include <vulkan/vulkan.hpp>
#include "Task.h"
#include "Devices/Device.h"

namespace Landmark
{
	namespace Vk
	{
		class Event_VulkanInstancePreInit : Events::Event {
		public:

		};




		
		class Event_GpuTaskRequest : Events::Event {
		public:
			struct TaskRequest {
				const std::string Name = "Default Task Name";
				const Task::TaskIntensities _type = Task::TaskIntensities::INVALID;
				const int DeviceID = -1;
				uint8_t RequestedCapabilities = 0b000000;
				const char Extensions[VK_MAX_EXTENSION_NAME_SIZE][50] = { {0} };
				const char Layers[VK_MAX_EXTENSION_NAME_SIZE][50] = { {0} };
			};
		private:
			struct FullTaskRequest {
				TaskRequest _ShallowRequest;
				Task* _task;
			};
			std::map<int, std::vector<FullTaskRequest>> DeviceRequests;
		public:
		
			const std::vector<Device> AvailableDevices;
			
			
			Task* DeclareTask(TaskRequest _request) {
				Task* newtask = new Task(_request._type);
				

				DeviceRequests[_request.DeviceID].push_back( FullTaskRequest{ _request,newtask });

				return newtask;
			}

			const std::map<int, std::vector<FullTaskRequest>> GetRequests() { return DeviceRequests; }
			
			Event_GpuTaskRequest();
		};
	}
}