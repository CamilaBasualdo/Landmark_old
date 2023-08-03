#include "Task.h"

#include "Devices/Device.h"
#include "Devices/Queue.h"

void Landmark::Vk::Task::Initiaize(Queue* _owner)
{
	Owner = _owner;

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = Owner->CmdPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = 1;

	if (vkAllocateCommandBuffers(Owner->Owner->GetVkDevice(), &allocInfo, &CmdBuffer) != VK_SUCCESS) {
		LOGGER.Error("Failed to allocate Command Buffer for " + Name);
	}
	else LOGGER.Log("Command Buffer Allocated for "+Name);
}

Landmark::Vk::Task::Task(std::string _Name, TaskIntensities _type): taskType(_type),Name(_Name)
{
}

void Landmark::Vk::Task::BeginRecord()
{
	VkCommandBufferBeginInfo info;
	info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	info.flags = 0;
	info.pInheritanceInfo = 0;
	info.pNext = nullptr;
	if (vkBeginCommandBuffer(CmdBuffer, &info) != VK_SUCCESS)
	{
		LOGGER.Error("Failed to Begin recording Command Buffer");
	}
	
}



void Landmark::Vk::Task::EndRecord()
{
	if (vkEndCommandBuffer(CmdBuffer) != VK_SUCCESS)
	{
		LOGGER.Error("Failed to End recording Command Buffer");
	}
}


