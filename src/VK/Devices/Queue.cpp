#include "Queue.h"

#include "Device.h"

Landmark::Vk::Queue::Queue(Device* _Owner, uint32_t _FamilyID, uint32_t _QueueID, std::vector<Task*> _Tasks)
:Owner(_Owner),FamilyID(_FamilyID),QueueID(_QueueID),
Tasks(_Tasks)
{
	vkGetDeviceQueue(Owner->GetVkDevice(), FamilyID, QueueID, &_Queue);
	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = FamilyID;


	if (vkCreateCommandPool(Owner->GetVkDevice(), &poolInfo, nullptr, &CmdPool) !=
		VK_SUCCESS) {
		LOGGER.Critical("Failed To Initialize Command pool. F:" + std::to_string(FamilyID) + ". Q:" + std::to_string(QueueID) + ". Device: " + Owner->_PhysicalDevice->Name);
		return;
	}
	else LOGGER.Log("Command Pool Created for F:" + std::to_string(FamilyID) + ". Q:" + std::to_string(QueueID) + ". Device: " + Owner->_PhysicalDevice->Name);

	for (auto& task : Tasks)
	{
		task->Initiaize(this);
	}


}
