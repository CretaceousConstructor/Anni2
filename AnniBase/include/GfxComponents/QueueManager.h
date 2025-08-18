#pragma once
#include "DeviceManager.h"
#include "GfxAPI/vulkan/AnniVulkan.h"
#include "WindowsSubSystem/glfw/WindowsSubsystem.h"

//#include "TimelineSemWrapper.h"

#include <ranges>
#include <unordered_map>
#include <unordered_set>
#include <expected>

namespace Anni2
{
	struct QueueCapability
	{
		bool   graphics{ false };
		bool   compute{ false };
		bool   transfer{ false };
		bool   present{ false };
		uint32_t queue_family_index{};
		uint32_t queue_index{};
	};

	class Queue
	{
	public:
		friend class QueueManager;
		Queue(vk::Queue queue_, QueueCapability queue_cap_, DeviceManager& device_man_);

		~Queue();

		Queue() = delete;
		Queue(const Queue&) = delete;
		Queue& operator=(const Queue&) = delete;

		Queue(Queue&& rhs) = delete;
		Queue& operator=(Queue&&) = delete;

		QueueCapability GetQueueCap() const;
		vk::Queue GetQueue() const;

		size_t GetNumDisbatchedPassesOnCurQueue();
		void SetNumDisbatchedPassesOnCurQueue(size_t n);

		std::pair<vk::CommandBuffer, vk::Semaphore> GetOneCmdBufAndAssociatedSem();
		void FinalizeGeneralRendering(std::vector<vk::Semaphore> waiting_sems, const uint32_t cur_frame, vk::Semaphore frame_num_semaphore);

		vk::Semaphore GetTimeLineSem();
		vk::CommandBuffer GetOneCmdBuf();
		std::pair<vk::CommandBuffer, Queue* const> BeginSingleTimeCommands();
		//void EndSingleTimeCopyCommands(std::pair<vk::CommandBuffer, Queue* const> cmd_buf_and_q, std::shared_ptr<TimelineSemWrapper> sem);

		vk::CommandPool CreateCommandPool(const vk::CommandPoolCreateFlagBits create_flag_bits);

	public:
		std::unordered_map<vk::CommandBuffer, vk::Semaphore> cmdbuf_to_sem;

	private:
		size_t                              num_disbatched_passes;
		vk::Queue                           queue;
		QueueCapability                     queue_cap;

		vk::UniqueSemaphore                 timeline_semaphore;

		std::vector<vk::UniqueCommandPool>  cmd_pools;
		vk::CommandPool                     main_transient_cmd_pool{ nullptr };
		vk::CommandPool                     main_resettable_cmd_pool{ nullptr };

		std::unordered_set<vk::CommandBuffer> cmd_bufs;

		DeviceManager& device_man;
	};

	class QueueManager
	{
	public:
		QueueManager(DeviceManager& device_man_, WindowsSys& window_);
		[[nodiscard]] Anni2::Queue& GetSuitableQueue(QueueCapability required_queue_cap);
		void                        SetAllQueuesNumDisbatchedPassesToZero();
		[[nodiscard]] Anni2::Queue& GetMostDedicatedTransferQueue();

	private:
		std::list<Anni2::Queue> queues;
	private:
		DeviceManager& device_man;
		WindowsSys& window;

	public:
		QueueManager() = delete;
		~QueueManager() = default;

		QueueManager(const QueueManager&) = delete;
		QueueManager& operator=(const QueueManager&) = delete;

		QueueManager(QueueManager&&) = delete;
		Queue& operator=(Queue&&) = delete;
	};

}        // namespace Anni