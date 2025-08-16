#pragma once
#include <vector>
#include <optional>
#include "Math/AnniMath.h"
#include "Buffer/BufferCreateInfo.h"
#include "GfxComponents/QueueManager.h"
#include "Memory/VmaAllocator.h"
#include "Synchronization/SyncInfo.h"

namespace Anni2
{
	class Buffer
	{
		friend class BufferFactory;
	public:
		using BufferPtrBundle = std::vector<std::shared_ptr<Buffer>>;
		using BufferPtr = std::shared_ptr<Buffer>;

	public:
		struct Descriptor
		{
			BufferCreateInfoEnhanced buf_CI;
			std::optional<vk::BufferViewCreateInfo> buf_view_CI;
			std::vector<uint8_t> init_cpu_data;
		};

		Buffer(
			QueueManager& queue_manager_,
			DeviceManager& device_manager_,
			Memory::VmaAllocatorWrapper& allocator_,
			//TimelineSemPoolUnsafe& sem_pool_,

			const vk::Buffer buffer_,
			const VmaAllocation vma_allocation_,
			const BufferCreateInfoEnhanced& buf_CI_
		);

		~Buffer();

	public:
		void          CopyFromHost(void const* outside_data_to_be_mapped, size_t outside_data_size, vk::DeviceSize mapped_region_starting_offset = 0);

	private:
		vk::Buffer                      buf;
		VmaAllocation                   vma_allocation{};
		BufferCreateInfoEnhanced        buf_CI;
	private:
		vk::DescriptorBufferInfo        des_buf_info;

	private:
		std::vector<Buf2BufCopyInfo>        copy_infos;
		BufSyncInfo                         sync_info_onload;

	private:
		QueueManager&            queue_manager;
		DeviceManager&           device_manager;
		Memory::VmaAllocatorWrapper&     allocator;
	};


}



