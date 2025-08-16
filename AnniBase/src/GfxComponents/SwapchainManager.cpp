#include "GfxComponents/SwapchainManager.h"

namespace Anni2
{
	SwapchainManager::SwapchainManager(DeviceManager& device_manager_, WindowsSys& window_, const uint32_t required_img_count_) :
		device_manager(device_manager_),
		window(window_),
		required_img_count(required_img_count_)
	{
		CreateSwapChainAndSwapImages();
	}

	void SwapchainManager::CreateSwapChainAndSwapImages()
	{
		const DeviceManager::SwapChainSupportDetails swapchain_support = DeviceManager::QuerySwapChainSupport(device_manager.GetPhysicalDevice(), window.GetSurface());
		//֧�ֵ�backbuffer��ʽ

		//format;
		//VK_FORMAT_B8G8R8A8_UNORM = 44,VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
		//VK_FORMAT_B8G8R8A8_SRGB =  50,VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
		//VK_FORMAT_A2B10G10R10_UNORM_PACK32 = 64,VK_COLOR_SPACE_SRGB_NONLINEAR_KHR

		//�ɵ�չʾ��ģʽ
		//VK_PRESENT_MODE_FIFO_KHR = 2,
		//VK_PRESENT_MODE_FIFO_RELAXED_KHR = 3,
		//VK_PRESENT_MODE_MAILBOX_KHR = 1,
		//VK_PRESENT_MODE_IMMEDIATE_KHR = 0,

		//ֻѡVK_FORMAT_B8G8R8A8_SRGB =  50,VK_COLOR_SPACE_SRGB_NONLINEAR_KHR, �������ֱ��throw exception
		surface_format = ChooseSwapSurfaceFormat(swapchain_support.formats);
		//������ѡ���ܹ�triple buffer��ģʽ
		present_mode = ChooseSwapPresentMode(swapchain_support.present_modes);
		swapchain_capabilities = swapchain_support.capabilities;

		const VkExtent2D extent = ChooseSwapExtent(swapchain_support.capabilities, window);

		//3����Ϊ��Ҫ3֡inflight
		image_count = swapchain_support.capabilities.minImageCount + 1;

		//swapChainSupport.capabilities.maxImageCount�������0��ʾû������
		if ( swapchain_support.capabilities.maxImageCount > 0 && image_count > swapchain_support.capabilities.maxImageCount )
		{
			image_count = swapchain_support.capabilities.maxImageCount;
		}

		ASSERT_WITH_MSG(required_img_count == image_count, "not enough swap chain images");

		vk::SwapchainCreateInfoKHR swapchain_CI{};
		swapchain_CI.flags = vk::SwapchainCreateFlagsKHR(VK_ZERO_FLAG);
		swapchain_CI.surface = window.GetSurface();
		swapchain_CI.minImageCount = image_count;
		swapchain_CI.imageFormat = surface_format.surfaceFormat.format;
		swapchain_CI.imageColorSpace = surface_format.surfaceFormat.colorSpace;
		swapchain_CI.imageExtent = extent;
		swapchain_CI.imageArrayLayers = 1;
		swapchain_CI.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;        //ֱ����image��Ⱦ��ɫ
		swapchain_CI.imageSharingMode = vk::SharingMode::eExclusive;        //����ֱ��������exclusive���������Լ������queue ownership transfer������
		//swapchain_CI.queueFamilyIndexCount = 0;                                // Optional
		//swapchain_CI.pQueueFamilyIndices = nullptr;                            // Optional

		swapchain_CI.preTransform = swapchain_support.capabilities.currentTransform;        //��ת90���������ת�����ȡ�����Ŀǰ����Կ�gtx1650���� ���� ���ⶼ��֧��
		swapchain_CI.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;                      //blending with another window

		swapchain_CI.presentMode = this->present_mode;
		swapchain_CI.clipped = VK_TRUE;               //�������������ס��vulkan���ڣ���ôvulkan���ڱ���ס�ĵط�pixel shader���ᱻ����
		swapchain_CI.oldSwapchain = VK_NULL_HANDLE;   //�������»��ƣ���ôswapchain���ܾ�ʧЧ�ˣ�ʧЧ��swapchain��Ҫ���������档��Ҫ�����µ�swapchain

		swap_chain = device_manager.GetLogicalDevice().createSwapchainKHRUnique(swapchain_CI).value;
		//==========================================================================================================
		//BGRA SRGB
		//swap_chain_image_format = surface_format.format;
		//swap_chain_image_view_format = surface_format.format;
		//std::vector<VkImageView> temp_swap_chain_image_views;  3��
		swap_chain_extent = extent;
		raw_swap_chain_images = device_manager.GetLogicalDevice().getSwapchainImagesKHR(swap_chain.get()).value;
	}

	vk::SurfaceFormatKHR SwapchainManager::ChooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& available_formats)
	{
		//if the SRGB color space is supported or not �� VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
		for ( const auto& available_format : available_formats )
		{
			if ( available_format.format == vk::Format::eB8G8R8A8Srgb && available_format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear )
			{
				return available_format;
			}
		}

		throw std::invalid_argument("can't find required format");
	}


	vk::PresentModeKHR SwapchainManager::ChooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& available_present_modes)
	{
		for ( const auto& availablePresentMode : available_present_modes )
		{
			//VK_PRESENT_MODE_MAILBOX_KHR:�ȴ���һ�ε�vertical blanking��ʱ��Ż����Ⱦ�õ�֡��front buffer���ڲ�����һ�����д����Ⱦ�õ�֡������������ˣ���������Ⱦ��֡�滻���������е�֡�����ұ��滻����Щ֡���Ա�Ӧ�ó����ٴ����á�
			//ѡVK_PRESENT_MODE_MAILBOX_KHR����triple buffer
			if ( availablePresentMode == vk::PresentModeKHR::eMailbox )
			{
				return availablePresentMode;
			}
		}

		//fallback�����:����û�в�֧��VK_PRESENT_MODE_FIFO_KHR��gpu
		return vk::PresentModeKHR::eFifo;
	}

	vk::Extent2D SwapchainManager::ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities, const WindowsSys& window)
	{
		if ( capabilities.currentExtent.width != UINT32_MAX )
		{
			return capabilities.currentExtent;
		}
		else
		{
			int width, height;
			glfwGetFramebufferSize(const_cast< GLFWwindow* >(window.GetWindowPtr()), &width, &height);
			vk::Extent2D actualExtent = {
				static_cast< uint32_t >(width),
				static_cast< uint32_t >(height)
			};

			//������������С֮��
			actualExtent.width = std::max<uint32_t>(capabilities.minImageExtent.width, std::min<uint32_t>(capabilities.maxImageExtent.width, actualExtent.width));
			actualExtent.height = std::max<uint32_t>(capabilities.minImageExtent.height, std::min<uint32_t>(capabilities.maxImageExtent.height, actualExtent.height));

			return actualExtent;
		}
	}


	vk::Format SwapchainManager::GetSwapChainImageFormat() const
	{
		return surface_format.surfaceFormat.format;
	}

	vk::Format SwapchainManager::GetSwapChainImageViewFormat() const
	{
		return surface_format.surfaceFormat.format;
	}

	vk::Extent3D SwapchainManager::GetSwapChainImageExtent() const
	{
		vk::Extent3D result;
		result.width = swap_chain_extent.width;
		result.height = swap_chain_extent.height;
		result.depth = 1;
		return result;
	}



	vk::Extent2D SwapchainManager::GetSwapChainImageExtent2D() const
	{
		vk::Extent2D result;
		result.width = swap_chain_extent.width;
		result.height = swap_chain_extent.height;

		return result;
	}


	vk::SurfaceFormatKHR SwapchainManager::GetSurfaceFormat() const
	{
		return surface_format.surfaceFormat;
	}

	vk::PresentModeKHR SwapchainManager::GetPresentMode() const
	{
		return present_mode;
	}

	uint32_t SwapchainManager::GetMinImageCount() const
	{
		return swapchain_capabilities.surfaceCapabilities.minImageCount;
	}

	vk::Format SwapchainManager::FindDepthFormat() const
	{
		return
			device_manager.FindSupportedFormat(
				{ vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint },
				vk::ImageTiling::eOptimal,
				vk::FormatFeatureFlagBits::eDepthStencilAttachment
			);
	}

	uint32_t SwapchainManager::GetSwapImageCount() const
	{
		return image_count;
	}

	vk::SwapchainKHR& SwapchainManager::GetSwapChain()
	{
		return *swap_chain;
	}

	const std::vector<vk::Image>& SwapchainManager::GetSwapChainImages() const
	{
		return raw_swap_chain_images;
	}

}        // namespace Anni
