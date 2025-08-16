
#include "Buffer/BufferCreateInfo.h"

namespace Anni2
{
	bool CI::IsSameType(const BufferCreateInfoEnhanced& lhs, const BufferCreateInfoEnhanced& rhs)
	{
		return
		(
			lhs.vk_buffer_CI.usage == rhs.vk_buffer_CI.usage &&
			lhs.vma_allocation_CI.usage == rhs.vma_allocation_CI.usage &&
			lhs.vma_allocation_CI.flags == rhs.vma_allocation_CI.flags
		);
	}

	BufferCreateInfoEnhanced::BufferCreateInfoEnhanced(
		vk::BufferUsageFlags usage_,
		VmaMemoryUsage vma_usage_,
		VmaAllocationCreateFlags vma_allo_flags)
	{
		vk_buffer_CI.usage = usage_;
		vma_allocation_CI.usage = vma_usage_;
		vma_allocation_CI.flags = vma_allo_flags;
	}

}
