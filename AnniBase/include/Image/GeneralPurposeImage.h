#pragma once
#include "Image/IImageBase.h"
#include "Memory\VmaAllocator.h"


namespace Anni2
{
	class GeneralPurposeImageReFac final : public IImageBase
	{
		friend class ImageFactory;
	public:
		GeneralPurposeImageReFac(
			GraphicsComponent& gfx_,
			Memory::VmaAllocatorWrapper& allocator,
			const vk::Image image_,
			const VmaAllocation  vma_allocation_,
			ImageCIEnhanced& vk_image_CI
			//TimelineSemPoolUnsafe& sem_pool_
		);

		~GeneralPurposeImageReFac() override;
		GeneralPurposeImageReFac() = delete;

		GeneralPurposeImageReFac(const GeneralPurposeImageReFac&) = delete;
		GeneralPurposeImageReFac& operator=(const GeneralPurposeImageReFac&) = delete;

		GeneralPurposeImageReFac(GeneralPurposeImageReFac&&) = delete;
		GeneralPurposeImageReFac& operator=(GeneralPurposeImageReFac&&) = delete;

		void CopyBufferToImageOnTransferUpload(vk::Buffer buffer_, const std::vector<vk::BufferImageCopy2>& bufferCopyRegions_);
		//void CopyBufferToImage1mip1levelOnTransferUpload(VkBuffer buffer, uint32_t width, uint32_t height);

	private:
		//VkDeviceMemory image_mem{ nullptr };
		Memory::VmaAllocatorWrapper& allocator;
		VmaAllocation        vma_allocation;
		//TimelineSemPoolUnsafe& sem_pool;
	};



}        // namespace Anni
