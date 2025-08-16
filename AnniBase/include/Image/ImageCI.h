#pragma once
#include "GfxComponents/SwapchainManager.h"
#include "GfxAPI/vulkan/AnniVulkan.h"

#include <vma/vk_mem_alloc.h>
#include <type_traits>
#include <functional>
#include <unordered_map>



namespace Anni2
{
	struct ImageCIEnhanced
	{
		explicit ImageCIEnhanced() = default;

		~ImageCIEnhanced() = default;
		ImageCIEnhanced(const ImageCIEnhanced&) = default;
		ImageCIEnhanced& operator=(const ImageCIEnhanced&) = default;

		ImageCIEnhanced(ImageCIEnhanced&&) = default;
		ImageCIEnhanced& operator=(ImageCIEnhanced&&) = default;

		vk::ImageCreateInfo     vk_image_CI;
		vk::ImageAspectFlags    aspect_flags;
		VmaAllocationCreateInfo vma_allocation_CI{};
		VmaAllocationInfo       vma_allocation_info{};

		std::optional<std::string> image_path;
	private:
		//vk::MemoryPropertyFlags mem_prop;
		//vk::MemoryRequirements  mem_req;
		//vk::MemoryAllocateInfo  mem_alloc_info;
	};

	namespace CI
	{
		ImageCIEnhanced GetDepthImgCI(const vk::Format format_, const vk::Extent3D& image_extend_, vk::ImageCreateFlags flags = vk::ImageCreateFlags(VK_ZERO_FLAG));
		ImageCIEnhanced GetDepthImgCI(SwapchainManager& swapchain_manager, vk::ImageCreateFlags flags = vk::ImageCreateFlags(VK_ZERO_FLAG));
		ImageCIEnhanced GetTextureImgCI(const vk::Format format_, const vk::Extent3D& image_extend_, vk::ImageCreateFlags flags = vk::ImageCreateFlags(VK_ZERO_FLAG), std::optional<std::string> image_path_ = std::nullopt);

		ImageCIEnhanced GetSwapchainImgCI();

	}        // namespace CI
	//***************************************************************
}        // namespace Anni

//namespace std
//{
//	template <>
//	struct hash <Anni::ImageCIEnhanced>
//	{
//		std::size_t operator()(const Anni::ImageCIEnhanced& obj) const noexcept
//		{
//			// Combine hash values of individual members using XOR
//			std::size_t seed = 0;
//
//			VULKAN_HPP_HASH_COMBINE(seed, obj.vk_image_CI);
//			VULKAN_HPP_HASH_COMBINE(seed, obj.aspect_flags);
//			VULKAN_HPP_HASH_COMBINE(seed, obj.mem_prop);
//
//			return seed;
//		}
//	};
//
//
//}











