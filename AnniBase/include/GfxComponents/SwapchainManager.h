#pragma once

#include "DeviceManager.h"
#include "GfxAPI/vulkan/AnniVulkan.h"
#include "Windows.h"
#include <iostream>
#include <set>
#include <vector>

namespace Anni2
{
	class SwapchainManager
	{
	public:
		//every surface has a swapchain associated with it.
		SwapchainManager(DeviceManager& device_manager_, WindowsSys& window_,const uint32_t required_img_count_);

		~SwapchainManager() = default;

		SwapchainManager(const SwapchainManager&) = delete;
		SwapchainManager& operator=(const SwapchainManager&) = delete;

		SwapchainManager(SwapchainManager&&) = delete;
		SwapchainManager& operator=(SwapchainManager&&) = delete;

		void                               CreateSwapChainAndSwapImages();
		[[nodiscard]] vk::Format           GetSwapChainImageFormat() const;
		[[nodiscard]] vk::Format           GetSwapChainImageViewFormat() const;
		[[nodiscard]] vk::Extent3D         GetSwapChainImageExtent() const;
		[[nodiscard]] vk::Extent2D         GetSwapChainImageExtent2D() const;
		[[nodiscard]] vk::SurfaceFormatKHR GetSurfaceFormat() const;
		[[nodiscard]] vk::PresentModeKHR   GetPresentMode() const;
		[[nodiscard]] uint32_t             GetMinImageCount() const;

		[[nodiscard]] vk::Format FindDepthFormat() const; //这个函数问题很大，需要重写
		[[nodiscard]] uint32_t   GetSwapImageCount() const;

		[[nodiscard]] vk::SwapchainKHR&              GetSwapChain() ;
		[[nodiscard]] const std::vector<vk::Image>& GetSwapChainImages() const;

	private:
		static vk::SurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
		static vk::PresentModeKHR    ChooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);
		static vk::Extent2D          ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities, const WindowsSys& window);

	private:
		vk::UniqueSwapchainKHR swap_chain;
	private:
		std::vector<vk::Image> raw_swap_chain_images;
		const uint32_t required_img_count;
		uint32_t                  image_count{};
		vk::Extent2D              swap_chain_extent{};
		vk::PresentModeKHR        present_mode{};
		vk::SurfaceFormat2KHR     surface_format{};
		vk::SurfaceCapabilities2KHR swapchain_capabilities{};

	private:
		DeviceManager& device_manager;
		WindowsSys& window;
	};

}        // namespace Anni
