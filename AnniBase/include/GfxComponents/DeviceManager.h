#pragma once
#include "GfxAPI/vulkan/AnniVulkan.h"
#include "GfxAPI/vulkan/ExtensionUtility.h"

#include "InstanceWrapper.h"
#include "WindowsSubSystem/WindowsSys.h"

#include <algorithm>
#include <array>
#include <list>
#include <ranges>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace Anni2
{
	class DeviceManager
	{
	public:
		explicit DeviceManager(InstanceWrapper& instance_, WindowsSys& window_);
		DeviceManager() = delete;
		~DeviceManager() = default;

		DeviceManager(const DeviceManager&) = delete;
		DeviceManager& operator=(const DeviceManager&) = delete;

		DeviceManager(DeviceManager&&) = delete;
		DeviceManager& operator=(DeviceManager&&) = delete;

	public:
		struct SwapChainSupportDetails
		{
			vk::SurfaceCapabilitiesKHR        capabilities{};
			std::vector<vk::SurfaceFormatKHR> formats;
			std::vector<vk::PresentModeKHR>   present_modes;
		};

		struct QueueFamilyIndices
		{
			std::optional<uint32_t> graphics_family;
			std::optional<uint32_t> present_family;
			std::optional<uint32_t> transfer_family;
			[[nodiscard]] bool      IsComplete() const;
		};

	public:
		static QueueFamilyIndices FindQueueFamilies(vk::PhysicalDevice phy_device, const vk::SurfaceKHR surface);

		static bool               IsDeviceSuitable(const vk::PhysicalDevice device, const vk::SurfaceKHR surface);
		static bool               CheckIfDeviceExtensionSupported(vk::PhysicalDevice device);

		static bool               CheckIfRequiredDepthFomatAndFeaturesSupported(vk::PhysicalDevice phy_device);

		static SwapChainSupportDetails QuerySwapChainSupport(vk::PhysicalDevice phy_device, vk::SurfaceKHR surface);
		static uint32_t                FindMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties, const vk::PhysicalDevice para_physical_device);


	public:
		[[nodiscard]] vk::Bool32 FormatIsFilterable(vk::Format format, vk::ImageTiling tiling) const;
		[[nodiscard]] vk::Format FindSupportedFormat(const std::vector<vk::Format> candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features) const;

		[[nodiscard]] vk::Device               GetLogicalDevice();
		[[nodiscard]] vk::PhysicalDevice       GetPhysicalDevice();
		const std::vector<vk::QueueFamilyProperties2>& GetQueueFamilyProps() const;
	private:
		void   PickOnePhysicalDevice();
		void   CreateLogicalDevice();

	private:
		static int RateDeviceSuitability(const VkPhysicalDevice& device_);
	private:
		std::list<VkCommandPool>             command_pools;
		std::vector<vk::QueueFamilyProperties2> queue_families_props;
		std::vector<std::vector<float>> queues_within_a_fam_priorities;

	private:
		InstanceWrapper& instance_wrapper;
		WindowsSys& window;

		vk::PhysicalDevice physical_device;
		vk::UniqueDevice  device;
	};
}        // namespace Anni
