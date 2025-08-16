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
		//支持的backbuffer格式

		//format;
		//VK_FORMAT_B8G8R8A8_UNORM = 44,VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
		//VK_FORMAT_B8G8R8A8_SRGB =  50,VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
		//VK_FORMAT_A2B10G10R10_UNORM_PACK32 = 64,VK_COLOR_SPACE_SRGB_NONLINEAR_KHR

		//可的展示的模式
		//VK_PRESENT_MODE_FIFO_KHR = 2,
		//VK_PRESENT_MODE_FIFO_RELAXED_KHR = 3,
		//VK_PRESENT_MODE_MAILBOX_KHR = 1,
		//VK_PRESENT_MODE_IMMEDIATE_KHR = 0,

		//只选VK_FORMAT_B8G8R8A8_SRGB =  50,VK_COLOR_SPACE_SRGB_NONLINEAR_KHR, 其他情况直接throw exception
		surface_format = ChooseSwapSurfaceFormat(swapchain_support.formats);
		//尽可能选择能够triple buffer的模式
		present_mode = ChooseSwapPresentMode(swapchain_support.present_modes);
		swapchain_capabilities = swapchain_support.capabilities;

		const VkExtent2D extent = ChooseSwapExtent(swapchain_support.capabilities, window);

		//3，因为想要3帧inflight
		image_count = swapchain_support.capabilities.minImageCount + 1;

		//swapChainSupport.capabilities.maxImageCount如果等于0表示没有限制
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
		swapchain_CI.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;        //直接往image渲染颜色
		swapchain_CI.imageSharingMode = vk::SharingMode::eExclusive;        //这里直接声明成exclusive，由我们自己来解决queue ownership transfer的问题
		//swapchain_CI.queueFamilyIndexCount = 0;                                // Optional
		//swapchain_CI.pQueueFamilyIndices = nullptr;                            // Optional

		swapchain_CI.preTransform = swapchain_support.capabilities.currentTransform;        //旋转90°操作，反转操作等。。。目前这个显卡gtx1650除了 不变 以外都不支持
		swapchain_CI.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;                      //blending with another window

		swapchain_CI.presentMode = this->present_mode;
		swapchain_CI.clipped = VK_TRUE;               //如果其他窗口遮住了vulkan窗口，那么vulkan窗口被遮住的地方pixel shader不会被调用
		swapchain_CI.oldSwapchain = VK_NULL_HANDLE;   //窗口重新绘制，那么swapchain可能就失效了，失效的swapchain需要存在这里面。你要创建新的swapchain

		swap_chain = device_manager.GetLogicalDevice().createSwapchainKHRUnique(swapchain_CI).value;
		//==========================================================================================================
		//BGRA SRGB
		//swap_chain_image_format = surface_format.format;
		//swap_chain_image_view_format = surface_format.format;
		//std::vector<VkImageView> temp_swap_chain_image_views;  3张
		swap_chain_extent = extent;
		raw_swap_chain_images = device_manager.GetLogicalDevice().getSwapchainImagesKHR(swap_chain.get()).value;
	}

	vk::SurfaceFormatKHR SwapchainManager::ChooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& available_formats)
	{
		//if the SRGB color space is supported or not ： VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
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
			//VK_PRESENT_MODE_MAILBOX_KHR:等待下一次的vertical blanking的时候才会把渲染好的帧给front buffer，内部存在一个队列存放渲染好的帧；如果队列满了，会用新渲染的帧替换队列中已有的帧，并且被替换的那些帧可以被应用程序再次利用。
			//选VK_PRESENT_MODE_MAILBOX_KHR用于triple buffer
			if ( availablePresentMode == vk::PresentModeKHR::eMailbox )
			{
				return availablePresentMode;
			}
		}

		//fallback的情况:几乎没有不支持VK_PRESENT_MODE_FIFO_KHR的gpu
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

			//控制在最大和最小之内
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
