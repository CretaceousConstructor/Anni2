#include "Buffer/Buffer.h"


namespace Anni2
{
	Buffer::Buffer(
		QueueManager& queue_manager_,
		DeviceManager& device_manager_,
		Memory::VmaAllocatorWrapper& allocator_,
		//TimelineSemPoolUnsafe& sem_pool_,

		const vk::Buffer buffer_,
		const VmaAllocation vma_allocation_,
		const BufferCreateInfoEnhanced& buf_CI_
	) :
		queue_manager(queue_manager_),
		device_manager(device_manager_),
		allocator(allocator_),
		//sem_pool(sem_pool_),
		buf(buffer_),
		vma_allocation(vma_allocation_),
		buf_CI(buf_CI_)
	{
	}

	Buffer::~Buffer()
	{
		vmaDestroyBuffer(allocator.GetRaw(), buf, vma_allocation);

		//Unmap();
		//device_manager.GetLogicalDevice().destroyBuffer(buf);
		//device_manager.GetLogicalDevice().freeMemory(buf_mem);
	}


	void Buffer::CopyFromHost(void const* outside_data_to_be_mapped, size_t outside_data_size, vk::DeviceSize mapped_region_starting_offset)
	{
		//必须要等待GPU端已经完成使用当前buffer！否则直接映射写数据一定会出问题
		for ( Buf2BufCopyInfo& history_cpy_inf : copy_infos | std::views::reverse )
		{
			if ( history_cpy_inf.queue &&  history_cpy_inf.trans_type != Buf2BufCopyInfo::TransferType::H2D)
			{
				const auto wait_value = history_cpy_inf.sem->GetLastValue() + 1;
				const vk::SemaphoreWaitInfo  sem_wait_info({}, history_cpy_inf.sem->GetRaw(), wait_value);

				const vk::Result result = device_manager.GetLogicalDevice().waitSemaphores(sem_wait_info, UINT64_MAX);
				switch ( result )
				{
					case vk::Result::eSuccess:
						break;
					default:
						// should not happen, as other result codes are considered to be an error and throw an exception
						ASSERT_WITH_MSG(false, "Failed to wait for semaphore!");
						break;
				}
				break;
			}
			//else branch是否需要barrier处理？？
		}

		VK_CHECK_RESULT(
			vmaCopyMemoryToAllocation(
				allocator.GetRaw(),
				outside_data_to_be_mapped,
				vma_allocation,
				mapped_region_starting_offset,
				outside_data_size
			));

		Buf2BufCopyInfo cpy_inf;
		cpy_inf.size = outside_data_size;
		cpy_inf.srcOffset = 0;
		cpy_inf.dstOffset = mapped_region_starting_offset;
		cpy_inf.trans_type = Buf2BufCopyInfo::TransferType::H2D;
		cpy_inf.sem = VK_NULL_HANDLE;
		copy_infos.push_back(std::move(cpy_inf));
	}



}















