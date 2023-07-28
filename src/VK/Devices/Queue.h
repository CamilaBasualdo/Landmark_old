#pragma once
#include <vulkan/vulkan.hpp>
#include "../../Logger.h"
#include "../Task.h"
namespace Landmark::Vk
{
	class Device;
	class Queue
	{
		friend Task;
		friend Device;

	protected:
		VkQueue _Queue = 0;
		VkCommandPool CmdPool = 0;
		const int FamilyID;
		const int QueueID;
		static inline Logger LOGGER = Logger("Vk::Queue");

		Device* Owner = nullptr;
		const std::vector<Task*> Tasks;

	public:
		Queue(Device* _Owner, uint32_t FamilyID, uint32_t QueueID,std::vector<Task*> Tasks);
	public:
		Queue(const Queue&) = default;

		Queue& operator=(const Queue&) = default;

		const int GetFamilyID() const { return FamilyID; }
		const int GetQueueID() const { return QueueID; }
		VkQueue GetQueue() const { return _Queue; }
	};


}
