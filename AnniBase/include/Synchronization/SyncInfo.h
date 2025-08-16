#pragma once
#include "GfxAPI/vulkan/AnniVulkan.h"
#include "GfxComponents/QueueManager.h"
#include "Synchronization/TimelineSemWrapper.h"

namespace Anni2
{
	struct WaitValue
	{
		auto operator<=>(const WaitValue& other) const = default;
		uint64_t val;
	};

	struct SignalValue
	{
		auto operator<=>(const SignalValue& other) const = default;
		uint64_t val;
	};

	//struct SemInsertInfoSafe
	//{
	//	SemInsertInfoSafe() = delete;
	//	SemInsertInfoSafe(WaitValue wait_val_, vk::PipelineStageFlags2 wait_stages_);
	//	SemInsertInfoSafe(SignalValue signal_val_, vk::PipelineStageFlags2 signal_stages_);
	//	SemInsertInfoSafe(SignalValue signal_val_, vk::PipelineStageFlags2 signal_stages_, WaitValue wait_val_, vk::PipelineStageFlags2 wait_stages_);

	//public:
	//	std::optional<WaitValue> wait_val;
	//	std::optional<SignalValue> signal_val;
	//	vk::PipelineStageFlags2 wait_stages;
	//	vk::PipelineStageFlags2 signal_stages;
	//};


	struct BufferSubRange
	{
		vk::DeviceSize offset;
		vk::DeviceSize size;
	};

	struct BufSyncInfo
	{
		vk::AccessFlags2        access_flags;
		vk::PipelineStageFlags2 stage_flags;
		std::optional<BufferSubRange> buf_subrange;
	};

	struct ImgSyncInfo
	{
		vk::AccessFlags2        access_flags;
		vk::PipelineStageFlags2 stage_flags;
		vk::ImageLayout         layout_inpass;

		std::optional<vk::ImageSubresourceRange> img_subrange;
	};

	struct Buf2BufCopyInfo
	{
		enum class TransferType
		{
			Unknown,
			H2D,
			D2H,
			D2D_DST,
			D2D_SRC
		};

		vk::DeviceSize size;
		vk::DeviceSize srcOffset;
		vk::DeviceSize dstOffset;
		TransferType trans_type;

		std::optional<Queue*> queue;
		std::shared_ptr<TimelineSemWrapper> sem;

	};



}        // namespace Anni


