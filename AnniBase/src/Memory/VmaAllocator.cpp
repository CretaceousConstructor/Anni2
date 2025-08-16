#include "Memory/VmaAllocator.h"



namespace Anni2::Memory
{

	Anni2::Memory::VmaAllocatorWrapper::VmaAllocatorWrapper(InstanceWrapper& instance_wrapper_, DeviceManager& device_manager_):
		instance_wrapper(instance_wrapper_),
		device_manager(device_manager_)
	{
		VmaAllocatorCreateInfo allocatorInfo = {};
		allocatorInfo.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;
		allocatorInfo.vulkanApiVersion = ANNI_VK_API_VERSION;
		allocatorInfo.physicalDevice = device_manager.GetPhysicalDevice();
		allocatorInfo.device = device_manager.GetLogicalDevice();
		allocatorInfo.instance = instance_wrapper.GetInstance();

		vmaCreateAllocator(&allocatorInfo, &allocator);
	}

	Anni2::Memory::VmaAllocatorWrapper::~VmaAllocatorWrapper()
	{
		vmaDestroyAllocator(allocator);
	}

	VmaAllocator& Anni2::Memory::VmaAllocatorWrapper::GetRaw()
	{
		return allocator;
	}
}
