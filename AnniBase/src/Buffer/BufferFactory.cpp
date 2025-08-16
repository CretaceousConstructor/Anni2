#include "Buffer/BufferFactory.h"

namespace Anni2
{
	BufferFactory::BufferFactory(
		QueueManager& queue_manager_,
		DeviceManager& device_manager_,
		Memory::VmaAllocatorWrapper& vam_allocator_,
		TimelineSemPoolUnsafe& sem_pool_
	) :
		queue_manager(queue_manager_), device_manager(device_manager_), vam_allocator(vam_allocator_), sem_pool(sem_pool_)
	{
	}


	std::shared_ptr<Buffer> BufferFactory::ProduceBuffer(vk::DeviceSize N, BufferCreateInfoEnhanced buf_CI, const void* const data)
	{
		buf_CI.vk_buffer_CI.size = N;
		//let the VMA library know that this data should be writeable by CPU, but also readable by GPU

		VkBuffer      temp_buffer{};
		VmaAllocation vma_allocation{};
		const VkBufferCreateInfo vk_buffer_CI = buf_CI.vk_buffer_CI;

		VK_CHECK_RESULT(
			vmaCreateBuffer(
				vam_allocator.GetRaw(),
				&vk_buffer_CI,
				&buf_CI.vma_allocation_CI,
				&temp_buffer,
				&vma_allocation,
				&buf_CI.vma_allocation_info
			));

		vk::Buffer result_buffer{ temp_buffer };

		//std::shared_ptr<Buffer> result = std::make_shared<Buffer>(queue_manager, device_manager, vam_allocator, sem_pool, result_buffer, vma_allocation, buf_CI);

		std::shared_ptr<Buffer> result = std::make_shared<Buffer>(queue_manager, device_manager, vam_allocator, result_buffer, vma_allocation, buf_CI);


		//把staging buf管理起来，免得拷贝没有完成就销毁。（异步拷贝）
		if ( CI::IsSameType(buf_CI, CI::StagingBuffer) )
		{
			if ( transient_buffers.size() > 100u ) //也可以维护一个transient staging buffer total size in use，但是为了方便，现在这么写。回收频率比较慢，
			{
				const auto& candidate_recycle_buf = transient_buffers.front();
				const bool all_signaled =
					std::ranges::all_of
					(candidate_recycle_buf->copy_infos,
					 [](const Buf2BufCopyInfo& buf_cp_inf)
					 {
						 return ((buf_cp_inf.sem->GetLastValue() + 1) == buf_cp_inf.sem->ReturnState());
					 }
					);

				if ( all_signaled )
				{
					transient_buffers.pop_front();
				}
			}
			transient_buffers.push_back(result);
		}

		if ( data )
		{
			result->CopyFromHost(data, N);
			result->sync_info_onload =
				BufSyncInfo{
					.access_flags = vk::AccessFlagBits2::eHostWrite,
					.stage_flags = vk::PipelineStageFlagBits2::eHost
			};
		}
		else
		{
			result->sync_info_onload =
				BufSyncInfo{
					.access_flags = vk::AccessFlagBits2::eNone,
					.stage_flags = vk::PipelineStageFlagBits2::eNone
			};
		}
		return result;
	}

	//void BufferFactory::ActualizeVirtualResource(RenderGraphV1::VirtualBuffer& vbuf)
	//{
	//	ASSERT_WITH_MSG(vbuf.descriptor.has_value(), "No discriptor of the given resource is provided.");

	//	const auto& dis = vbuf.descriptor.value();
	//	vbuf.p_rsrc = ProduceBuffer(dis.buf_CI, dis.init_cpu_data);
	//}


	std::shared_ptr<Buffer> BufferFactory::ProduceBuffer(BufferCreateInfoEnhanced buf_CI, const void* const data)
	{
		return ProduceBuffer(buf_CI.vk_buffer_CI.size, buf_CI, data);
	}

	Buffer::BufferPtrBundle BufferFactory::ProduceBufferPtrArray(VkDeviceSize N, uint32_t bundle_size, BufferCreateInfoEnhanced buf_CI)
	{
		std::vector<std::shared_ptr<Buffer>> result_bundle;
		for ( size_t i = 0; i < bundle_size; i++ )
		{
			result_bundle.push_back(ProduceBuffer(N, buf_CI));
		}
		return result_bundle;
	}




}        // namespace Anni
