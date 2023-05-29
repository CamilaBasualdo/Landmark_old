#pragma once
#include <map>

#include "../Events/Event.h"
#include <vulkan/vulkan.h>
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
				const Task::TaskTypes _type = Task::TaskTypes::INVALID;
				const int DeviceID = -1;
				uint8_t RequestedCapabilities = 0b00000;
				//const bool Graphics = false, Compute = false, Transfer= false, SparseBinding = false, VideoDecoding = false;
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
		
			const std::vector<Devices::Device> AvailableDevices;
			
			
			Task* DeclareTask(TaskRequest _request) {
				char* Buffer = new char[sizeof(Task)];
				auto TaskPTR = reinterpret_cast<Task*>(Buffer);
				

				DeviceRequests[_request.DeviceID].push_back( FullTaskRequest{ _request,TaskPTR });

				return TaskPTR;
			}

			const std::map<int, std::vector<FullTaskRequest>> GetRequests() { return DeviceRequests; }
			
			Event_GpuTaskRequest();
		};
	}
}