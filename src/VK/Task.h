#pragma once
#include <vulkan/vulkan.hpp>
#include <string>
namespace Landmark {
	namespace Vk {

		
		class Task
		{
			
		public:
			using CapabilitiesMask = uint8_t;
			enum TaskTypes {
				INVALID = -1,
				CONTINUOUS = 0,
				BURST = 1,
				SLEEPER = 2
			};
			const TaskTypes taskType;

			enum Capabilities {
				PRESENT = 0b0,
				GRAPHICS = 0b10,
				COMPUTE =0b100,
				TRANSFER=0b1000,
				SPARSE_BINDING=0b10000 
			};
		private:
			//TaskInfo _taskinfo;
			bool Initialized = false;
			
			CapabilitiesMask Capabilities = 0;

		protected:
			
			void Initiaize();
		public:
			Task(TaskTypes _type);
			bool IsInitialized() { return Initialized; }
			void Begin();

			void End();



			void Upload();

		};
	}
}


