#include "GfxComponents/QueueManager.h"

namespace Anni2
{
	Queue& QueueManager::GetSuitableQueue(QueueCapability required_queue_cap)
	{
		std::vector<Anni2::Queue*> suitable_queue;

		for ( auto& queue : queues )
		{
			const auto cur_queue_cap = queue.GetQueueCap();
			if ( required_queue_cap.graphics && !cur_queue_cap.graphics )
			{
				continue;
			}
			if ( required_queue_cap.compute && !cur_queue_cap.compute )
			{
				continue;
			}
			if ( required_queue_cap.transfer && !cur_queue_cap.transfer )
			{
				continue;
			}
			if ( required_queue_cap.present && !cur_queue_cap.present )
			{
				continue;
			}
			suitable_queue.push_back(&queue);
		}

		std::ranges::stable_sort(suitable_queue, [](const auto& rhs, const auto& lhs) //排序，然后找到负载最小的
								 {
									 return rhs->GetNumDisbatchedPassesOnCurQueue() < lhs->GetNumDisbatchedPassesOnCurQueue();
								 });

		Anni2::Queue* result = suitable_queue.front();
		return *result;
	}


	void QueueManager::SetAllQueuesNumDisbatchedPassesToZero()
	{
		for ( auto& queue : queues )
		{
			queue.SetNumDisbatchedPassesOnCurQueue(0);
		}
	}



	Queue& QueueManager::GetMostDedicatedTransferQueue()
	{
		std::vector<std::pair<Anni2::Queue*, size_t>> candidate;

		for ( auto& queue : queues )
		{
			size_t      score = 0;
			const auto& queue_cap = queue.GetQueueCap();
			if ( queue_cap.present )
				score++;
			if ( queue_cap.compute )
				score++;
			if ( queue_cap.graphics )
				score++;
			if ( queue_cap.transfer )
				score++;

			if ( queue_cap.transfer )
			{
				candidate.push_back(std::make_pair(&queue, score));
			}
		}
		std::ranges::sort(candidate,
						  [](std::pair<Anni2::Queue*, size_t>& lhs, std::pair<Anni2::Queue*, size_t>& rhs)
						  {
							  return lhs.second < rhs.second;
						  });

		Anni2::Queue* p_queue = candidate.front().first;
		return *p_queue;
	}

	QueueManager::QueueManager(DeviceManager& device_man_, WindowsSys& window_) :
		device_man(device_man_), window(window_)
	{
		const auto& queue_families_props = device_man.GetQueueFamilyProps();
		for ( const auto [i, queue_families_prop] : std::ranges::views::enumerate(queue_families_props) )
		{
			QueueCapability capability;
			//FILL UP QUEUE CAPABILITY
			capability.queue_family_index = i;
			capability.present = device_man.GetPhysicalDevice().getSurfaceSupportKHR(i, window.GetSurface()).value;

			if ( queue_families_prop.queueFamilyProperties.queueFlags & vk::QueueFlagBits::eGraphics )
			{
				capability.graphics = true;
			}
			if ( queue_families_prop.queueFamilyProperties.queueFlags & vk::QueueFlagBits::eCompute )
			{
				capability.compute = true;
			}
			if ( queue_families_prop.queueFamilyProperties.queueFlags & vk::QueueFlagBits::eTransfer )
			{
				capability.transfer = true;
			}

			//每种队列家族创建了尽可能多个queue
			for ( int j = 0; j < queue_families_prop.queueFamilyProperties.queueCount; ++j )
			{
				vk::Queue temp_queue;
				capability.queue_index = j;
				temp_queue = device_man.GetLogicalDevice().getQueue(i, j);
				queues.emplace_back(temp_queue, capability, device_man);
			}

		}

	}

	Queue::Queue(vk::Queue queue_, QueueCapability queue_cap_, DeviceManager& device_man_) :
		num_disbatched_passes(0u),
		queue(queue_),
		queue_cap(queue_cap_),
		device_man(device_man_)
	{
		const vk::Device logical_device = device_man.GetLogicalDevice();
		//*****************************************************************
		constexpr vk::SemaphoreTypeCreateInfo timelineCreateInfo(vk::SemaphoreType::eTimeline, 0, VK_NULL_HANDLE);
		const vk::SemaphoreCreateInfo createInfo(vk::SemaphoreCreateFlags(VK_ZERO_FLAG), &timelineCreateInfo);
		auto [result0, timeline_semaphore_tmp] = logical_device.createSemaphoreUnique(createInfo);
		assert(vk::Result::eSuccess == result0);
		timeline_semaphore = std::move(timeline_semaphore_tmp);
		//*****************************************************************
		const vk::CommandPoolCreateInfo transient_command_pool_CI(
			vk::CommandPoolCreateFlagBits::eTransient,
			queue_cap.queue_family_index,
			VK_NULL_HANDLE
		);
		auto [result1, tmp_cmd_pool_transient] = logical_device.createCommandPoolUnique(transient_command_pool_CI);
		assert(vk::Result::eSuccess == result1);
		main_transient_cmd_pool = tmp_cmd_pool_transient.get();
		cmd_pools.emplace_back(std::move(tmp_cmd_pool_transient));
		//*****************************************************************

		const vk::CommandPoolCreateInfo resettable_command_pool_CI(
			vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
			queue_cap.queue_family_index,
			VK_NULL_HANDLE
		);
		auto [result1, tmp_cmd_pool_resettable] = logical_device.createCommandPoolUnique(resettable_command_pool_CI);
		assert(vk::Result::eSuccess == result1);
		main_resettable_cmd_pool = tmp_cmd_pool_resettable.get();
		cmd_pools.emplace_back(std::move(tmp_cmd_pool_resettable));
		//*****************************************************************
	}

	vk::CommandPool Queue::CreateCommandPool(const vk::CommandPoolCreateFlagBits create_flag_bits) 
	{
		const vk::Device logical_device = device_man.GetLogicalDevice();
		//*****************************************************************
		const vk::CommandPoolCreateInfo command_pool_CI(
			create_flag_bits,
			queue_cap.queue_family_index,
			VK_NULL_HANDLE
		);

		auto [result, tmp_cmd_pool] = logical_device.createCommandPoolUnique(command_pool_CI);
		assert(vk::Result::eSuccess == result);
		const auto result_ptr = tmp_cmd_pool.get();
		cmd_pools.emplace_back(std::move(tmp_cmd_pool));
		return result_ptr;
	}




	Queue::~Queue()
	{
		for ( const auto& [_, sem] : cmdbuf_to_sem )
		{
			device_man.GetLogicalDevice().destroySemaphore(sem);
		}

	}

	QueueCapability Queue::GetQueueCap() const
	{
		return queue_cap;
	}

	vk::Queue Queue::GetQueue() const
	{
		return queue;
	}

	size_t Queue::GetNumDisbatchedPassesOnCurQueue()
	{
		return num_disbatched_passes;
	}

	void Queue::SetNumDisbatchedPassesOnCurQueue(size_t n)
	{
		num_disbatched_passes = n;
	}



	std::pair<vk::CommandBuffer, vk::Semaphore> Queue::GetOneCmdBufAndAssociatedSem()
	{
		//get COMMAND BUFFER
		vk::CommandBufferAllocateInfo buffer_alloc_info
		(
			cmd_pool.get(),
			vk::CommandBufferLevel::ePrimary,
			1
		);

		std::vector<vk::CommandBuffer> temp_bufs = device_man.GetLogicalDevice().allocateCommandBuffers(buffer_alloc_info).value;

		//get Timeline Semaphore
		//*****************************************************************
		vk::SemaphoreTypeCreateInfo timelineCreateInfo(vk::SemaphoreType::eTimeline, 0, VK_NULL_HANDLE);
		vk::SemaphoreCreateInfo createInfo(vk::SemaphoreCreateFlags(VK_ZERO_FLAG), &timelineCreateInfo);
		vk::Semaphore temp_sem = device_man.GetLogicalDevice().createSemaphore(createInfo).value;

		cmdbuf_to_sem.emplace(temp_bufs.front(), temp_sem);
		return { temp_bufs.front(), temp_sem };
	}




	void Queue::FinalizeGeneralRendering(std::vector<vk::Semaphore> waiting_sems, const uint32_t cur_frame, vk::Semaphore frame_num_semaphore)
	{
		ASSERT_WITH_MSG(false, "not yet implemented");

		std::vector<vk::SemaphoreSubmitInfo> wait_sem_infos;

		std::ranges::transform
		(waiting_sems, std::back_inserter(wait_sem_infos),
		 [](vk::Semaphore& sem)
		 {
			 return VkSemaphoreSubmitInfo{
				 .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
				 .pNext = VK_NULL_HANDLE,
				 .semaphore = sem,
				 .value = 1,
				 .stageMask = VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT,    //optimization later
				 .deviceIndex = 0
			 };
		 }
		);

		std::vector<vk::SemaphoreSubmitInfo> signal_sem_infos;

		signal_sem_infos.push_back(
			vk::SemaphoreSubmitInfo(frame_num_semaphore, cur_frame + 1, vk::PipelineStageFlagBits2::eAllCommands)
		);



		vk::SubmitInfo2 submit_info{};
		submit_info.setWaitSemaphoreInfos(wait_sem_infos)
			.setSignalSemaphoreInfos(signal_sem_infos);

		queue.submit2(submit_info);
	};


	vk::Semaphore Queue::GetTimeLineSem()
	{
		return *timeline_semaphore;
	}

	vk::CommandBuffer Queue::GetOneCmdBuf()
	{
		//get BUFFER
		vk::CommandBufferAllocateInfo cmd_buf_alloc_info{};

		cmd_buf_alloc_info.level = vk::CommandBufferLevel::ePrimary;
		cmd_buf_alloc_info.commandPool = cmd_pool.get();
		cmd_buf_alloc_info.commandBufferCount = 1;

		std::vector<vk::CommandBuffer> temp_buf = device_man.GetLogicalDevice().allocateCommandBuffers(cmd_buf_alloc_info).value;

		cmd_bufs.insert(temp_buf.front());

		return temp_buf.front();
	}

	std::pair<vk::CommandBuffer, Queue* const> Queue::BeginSingleTimeCommands()
	{
		/*typedef struct VkCommandBufferAllocateInfo {
				VkStructureType         sType;
				const void* pNext;      gotta be nullptr
				VkCommandPool           commandPool;
				VkCommandBufferLevel    level;
				uint32_t                commandBufferCount;
			} VkCommandBufferAllocateInfo;*/


		vk::CommandBufferAllocateInfo cmd_buf_alloc_info{};

		cmd_buf_alloc_info.level = vk::CommandBufferLevel::ePrimary;
		cmd_buf_alloc_info.commandPool = cmd_pool.get();
		cmd_buf_alloc_info.commandBufferCount = 1;

		std::vector<vk::CommandBuffer> temp_buf = device_man.GetLogicalDevice().allocateCommandBuffers(cmd_buf_alloc_info).value;

		const vk::CommandBufferBeginInfo begin_info(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

		temp_buf.front().begin(begin_info);

		return { temp_buf.front(), this };
	}

	/*
	void Queue::EndSingleTimeCopyCommands(std::pair<vk::CommandBuffer, Queue* const> cmd_buf_and_q,
										  std::shared_ptr<TimelineSemWrapper> sem)
	{
		//ASSERT_WITH_MSG(cmd_buf_and_q.second == this);
		//cmd_buf_and_q.first.end();

		//vk::SemaphoreSubmitInfo sig_smt_inf{};
		//sig_smt_inf.semaphore = sem->GetRaw(),
		//sig_smt_inf.value = sem->GetLastValue() + 1;
		//sig_smt_inf.stageMask = vk::PipelineStageFlagBits2::eCopy;

		//vk::CommandBufferSubmitInfo buf_smt_inf(cmd_buf_and_q.first);

		//vk::SubmitInfo2 smt_inf{};
		//smt_inf.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2;
		//smt_inf.pNext = VK_NULL_HANDLE;
		//smt_inf.flags = Vk::NoneFlag;
		//smt_inf.waitSemaphoreInfoCount = 0;
		//smt_inf.pWaitSemaphoreInfos = VK_NULL_HANDLE;



		//smt_inf.commandBufferInfoCount = 1;
		//smt_inf.pCommandBufferInfos = &buf_smt_inf;

		//smt_inf.signalSemaphoreInfoCount = 1;
		//smt_inf.pSignalSemaphoreInfos = &sig_smt_inf;

		//VK_CHECK_RESULT(vkQueueSubmit2(queue, 1, &smt_inf, nullptr));

		//TODO: command buffer回收
		//vkFreeCommandBuffers(device, command_pool, 1, &command_buffer);
	}

	k*/
}        // namespace Anni
