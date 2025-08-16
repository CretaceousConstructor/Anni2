#include "Image/ImageFactory.h"

//namespace Anni2
//{
//	ImageFactory::ImageFactory(
//		GraphicsComponent& gfx_,
//		VmaAllocatorWrapper& vam_allocator_,
//		TimelineSemPoolUnsafe& sem_pool_
//	) :
//		gfx(gfx_),
//		device_manager(gfx.DeviceMan()),
//		swapchain_manager(gfx.SwapchainMan()),
//		sem_pool(sem_pool_),
//		vam_allocator(vam_allocator_)
//	{
//	}
//
//	std::shared_ptr<GeneralPurposeImageReFac> ImageFactory::ProduceImage(ImageCIEnhanced& img_CI) const
//	{
//		VkImage temp_image{};
//		VmaAllocation vma_allocation{};
//		VkImageCreateInfo vk_image_CI = img_CI.vk_image_CI;
//
//		VK_CHECK_RESULT(vmaCreateImage(
//			vam_allocator.GetRaw(),
//			&vk_image_CI,
//			&img_CI.vma_allocation_CI,
//			&temp_image,
//			&vma_allocation,
//			&img_CI.vma_allocation_info
//		));
//
//		vk::Image result_image{temp_image};
//
//		auto result = std::make_shared<GeneralPurposeImageReFac>(gfx, vam_allocator, result_image, vma_allocation, img_CI, sem_pool);
//
//		return result;
//	}
//
//	//vk::Image ImageFactory::BuildImage(ImageCIEnhanced& vk_image_CI) const
//	//{
//	//	vk::Image temp_image = device_manager.GetLogicalDevice().createImage(vk_image_CI.vk_image_CI);
//
//	//	return temp_image;
//	//}
//
//	//vk::DeviceMemory ImageFactory::CreateAndBindMemory(ImageCIEnhanced& vk_image_CI, vk::Image temp_image) const
//	//{
//	//	vk_image_CI.mem_req = device_manager.GetLogicalDevice().getImageMemoryRequirements(temp_image);
//
//	//	vk_image_CI.mem_alloc_info.allocationSize = vk_image_CI.mem_req.size;
//	//	vk_image_CI.mem_alloc_info.memoryTypeIndex = DeviceManager::FindMemoryType(vk_image_CI.mem_req.memoryTypeBits, vk_image_CI.mem_prop, device_manager.GetPhysicalDevice());        //找到可以分配VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT内存类型的index
//	//	vk::DeviceMemory temp_image_mem = device_manager.GetLogicalDevice().allocateMemory(vk_image_CI.mem_alloc_info);
//
//	//	device_manager.GetLogicalDevice().bindImageMemory(temp_image, temp_image_mem, 0);
//	//	return temp_image_mem;
//	//}
//
//	//void ImageFactory::TransitionImageLayout(ImageCIReFac &para_pack, const std::shared_ptr<IImageBase> &result)
//	//{
//	//	if (para_pack.default_layout_right_aft_creation == VK_IMAGE_LAYOUT_UNDEFINED)
//	//	{
//	//		return;
//	//	}
//	//	else if (para_pack.default_layout_right_aft_creation == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
//	//	{
//	//		VkImageSubresourceRange subresource_range;
//	//		subresource_range.baseMipLevel   = 0;
//	//		subresource_range.levelCount     = para_pack.default_image_CI.mipLevels;
//	//		subresource_range.baseArrayLayer = 0;
//	//		subresource_range.layerCount     = para_pack.default_image_CI.arrayLayers;
//	//
//	//		result->TransitionImageLayout(VK_IMAGE_LAYOUT_UNDEFINED, para_pack.default_layout_right_aft_creation, DeviceManager::CommandPoolType::transfor_command_pool, subresource_range);
//	//	}
//	//	else
//	//	{
//	//		VkImageSubresourceRange subresource_range;
//	//		subresource_range.baseMipLevel   = 0;
//	//		subresource_range.levelCount     = para_pack.default_image_CI.mipLevels;
//	//		subresource_range.baseArrayLayer = 0;
//	//		subresource_range.layerCount     = para_pack.default_image_CI.arrayLayers;
//	//		result->TransitionImageLayout(VK_IMAGE_LAYOUT_UNDEFINED, para_pack.default_layout_right_aft_creation, DeviceManager::CommandPoolType::graphics_command_pool, subresource_range);
//	//	}
//	//
//	//	return;
//	//}
//
//}        // namespace Anni
