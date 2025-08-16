#pragma once
#include "DeviceManager.h"
#include "InstanceWrapper.h"
#include "QueueManager.h"

#include "GfxComponents/SwapchainManager.h"
#include "Windows.h"

namespace Anni2
{
	class GraphicsComponent
	{
	public:
		[[nodiscard]] InstanceWrapper& Instance();
		[[nodiscard]] WindowsSys& Window();
		[[nodiscard]] DeviceManager& DeviceMan();
		[[nodiscard]] QueueManager& QueueMan();
		[[nodiscard]] SwapchainManager& SwapchainMan();


	private:
		InstanceWrapper& instance;
		DeviceManager& device_manager;
		WindowsSys& window;
		QueueManager& queue_manager;
		SwapchainManager& swapchain_manager;

	public:
		GraphicsComponent(InstanceWrapper& instance_, DeviceManager& device_manager_,
			QueueManager& queue_manager_,  SwapchainManager& swapchain_manager_, WindowsSys& window_);

	public:
		~GraphicsComponent() = default;
		GraphicsComponent() = delete;

		GraphicsComponent(const GraphicsComponent&) = delete;
		GraphicsComponent& operator=(const GraphicsComponent&) = delete;

		GraphicsComponent(GraphicsComponent&&) = delete;
		GraphicsComponent& operator=(GraphicsComponent&&) = delete;
	};

}        // namespace Anni
