#pragma once
#include "IVirtualResource.h"
#include "Buffer/Buffer.h"


namespace Anni2::RenderGraph
{
	class VirtualBuffer : public IVirtualResource
	{
	public:
		struct Handle
		{
			auto operator<=>(const Handle&) const = default;
			Handle();

			explicit Handle(uint64_t handle_);
			uint64_t handle{ ~uint64_t(0x0) };
		};

		VirtualBuffer(std::string name_, std::shared_ptr<Buffer> ptr_rsrc_);
		VirtualBuffer(std::string name_, Buffer::Descriptor descriptor_);
		VirtualBuffer() = delete;

		VirtualBuffer(const VirtualBuffer&) = default;
		VirtualBuffer& operator=(const VirtualBuffer&) = delete;
		VirtualBuffer(VirtualBuffer&&) = default;
		VirtualBuffer& operator=(VirtualBuffer&&) = delete;
		~VirtualBuffer() override = default;


		VkBufferMemoryBarrier2 GetBufBarrier(const Anni2::BufSyncInfo& source_syn, const Anni2::BufSyncInfo& target_sync) const;

		std::shared_ptr<Buffer> p_rsrc;
		std::optional<Buffer::Descriptor> descriptor;
	};
	using VBufHandle = VirtualBuffer::Handle;


}
