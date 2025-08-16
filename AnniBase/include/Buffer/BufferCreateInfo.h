#pragma once
#include "GfxAPI/vulkan/AnniVulkan.h"
#include <vma/vk_mem_alloc.h>

namespace Anni2
{
	struct BufferCreateInfoEnhanced
	{
		BufferCreateInfoEnhanced() = delete;

		//constexpr BufferCreateInfoEnhanced(
		//	vk::BufferUsageFlags    usage_,
		//	vk::SharingMode         sharing_mode_,
		//	vk::MemoryPropertyFlags memory_properties_
		//) :
		//	mem_prop(memory_properties_)
		//{
		//	vk_buffer_CI.usage = usage_;
		//	vk_buffer_CI.sharingMode = sharing_mode_;
		//}

		BufferCreateInfoEnhanced(
			vk::BufferUsageFlags    usage_,
			VmaMemoryUsage          vma_usage_,
			VmaAllocationCreateFlags vma_allo_flags = static_cast< VmaAllocationCreateFlags >(VK_ZERO_FLAG)
		);

		~BufferCreateInfoEnhanced() = default;
		BufferCreateInfoEnhanced(const BufferCreateInfoEnhanced&) = default;
		BufferCreateInfoEnhanced& operator=(const BufferCreateInfoEnhanced&) = default;

		BufferCreateInfoEnhanced(BufferCreateInfoEnhanced&&) = default;
		BufferCreateInfoEnhanced& operator=(BufferCreateInfoEnhanced&&) = default;

		vk::BufferCreateInfo    vk_buffer_CI{};
		VmaAllocationCreateInfo vma_allocation_CI{};
		VmaAllocationInfo       vma_allocation_info{};

		//private:
		//unused because we already have VMA library
		//vk::MemoryPropertyFlags mem_prop{};
		//vk::MemoryRequirements  mem_req{};
		//vk::MemoryAllocateInfo  mem_alloc_info{};

	};


	namespace CI
	{
		bool IsSameType(const BufferCreateInfoEnhanced& lhs, const BufferCreateInfoEnhanced& rhs);

		const BufferCreateInfoEnhanced StagingBuffer
		{
			vk::BufferUsageFlagBits::eTransferSrc,
			VmaMemoryUsage::VMA_MEMORY_USAGE_AUTO,
			VmaAllocationCreateFlagBits::VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
			//vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
		};

		const BufferCreateInfoEnhanced UniformBuffer
		{
			vk::BufferUsageFlagBits::eUniformBuffer,
			VmaMemoryUsage::VMA_MEMORY_USAGE_AUTO,
			VmaAllocationCreateFlagBits::VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
			//vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
		};

		const BufferCreateInfoEnhanced VertexBuffer
		{
			vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer,
			VmaMemoryUsage::VMA_MEMORY_USAGE_AUTO
			//vk::MemoryPropertyFlagBits::eDeviceLocal
		};

		const BufferCreateInfoEnhanced StorageBufferAddressable
		{
			vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eShaderDeviceAddress,
			VmaMemoryUsage::VMA_MEMORY_USAGE_AUTO
			//vk::MemoryPropertyFlagBits::eDeviceLocal
		};

		const BufferCreateInfoEnhanced IndexBuffer
		{
			vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer,
			VmaMemoryUsage::VMA_MEMORY_USAGE_AUTO
			//vk::MemoryPropertyFlagBits::eDeviceLocal
		};


		//constexpr BufferCreateInfoEnhanced StagingBuffer
		//{
		//	vk::BufferUsageFlagBits::eTransferSrc,
		//	vk::SharingMode::eExclusive,
		//	vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
		//};

		//constexpr BufferCreateInfoEnhanced UniformBuffer
		//{
		//	vk::BufferUsageFlagBits::eUniformBuffer,
		//	vk::SharingMode::eExclusive,
		//	vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
		//};

		//constexpr BufferCreateInfoEnhanced VertexBuffer{
		//	vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer,
		//	vk::SharingMode::eExclusive,
		//	vk::MemoryPropertyFlagBits::eDeviceLocal };

		//constexpr BufferCreateInfoEnhanced StorageBufferAddressable
		//{
		//	vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eShaderDeviceAddress,
		//	vk::SharingMode::eExclusive,
		//	vk::MemoryPropertyFlagBits::eDeviceLocal
		//};

		//constexpr BufferCreateInfoEnhanced IndexBuffer
		//{
		//	vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer,
		//	vk::SharingMode::eExclusive,
		//	vk::MemoryPropertyFlagBits::eDeviceLocal
		//};





		//constexpr BufferCreateInfoEnhanced VertexBufferNonCoherent{VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT};
		//constexpr BufferCreateInfoEnhanced IndexBufferNonCoherent{VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT};

	}        // namespace CI

}

