#pragma once
#include "GfxComponents/GraphicsComponent.h"
#include "Buffer.h"
#include "BufferCreateInfo.h"
#include "Memory/VmaAllocator.h"
#include "Synchronization/TimelineSemPoolUnsafe.h"
#include <list>
#include <unordered_map>
#include <ranges>
//#include "VirtualBuffer.h"


namespace Anni2
{
	class BufferFactory
	{
	public:
		
		[[nodiscard]] Buffer::BufferPtrBundle ProduceBufferPtrArray(vk::DeviceSize N, uint32_t bundle_size, BufferCreateInfoEnhanced para_pack);
		[[nodiscard]] Buffer::BufferPtr ProduceBuffer(vk::DeviceSize N, BufferCreateInfoEnhanced buf_CI, const void* const data = nullptr);

	public:
		//void ActualizeVirtualResource(RenderGraphV1::VirtualBuffer& vbuf);

	private:
		[[nodiscard]] std::shared_ptr<Buffer> ProduceBuffer(BufferCreateInfoEnhanced buf_CI, const void* const data = nullptr);


	private:
		std::list<std::shared_ptr<Buffer>> transient_buffers;

	private:
		QueueManager& queue_manager;
		DeviceManager& device_manager;
		Memory::VmaAllocatorWrapper& vam_allocator;
		TimelineSemPoolUnsafe& sem_pool;

	public:
		BufferFactory(
			QueueManager& queue_manager_,
			DeviceManager& device_manager_,
			Memory::VmaAllocatorWrapper& vam_allocator_,
			TimelineSemPoolUnsafe& sem_pool_
		);
		~BufferFactory() = default;

		BufferFactory() = delete;
		BufferFactory(const BufferFactory&) = delete;
		BufferFactory& operator=(const BufferFactory&) = delete;
		BufferFactory(BufferFactory&&) = delete;
		BufferFactory& operator=(BufferFactory&&) = delete;

	};

}        // namespace Anni
