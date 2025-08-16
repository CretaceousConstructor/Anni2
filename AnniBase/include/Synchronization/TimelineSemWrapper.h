#pragma once
#include "GfxComponents/DeviceManager.h"

namespace Anni2
{
	class TimelineSemWrapper
	{
		friend class TimelineSemPoolUnsafe;
	public:
		uint64_t       ReturnState();
		vk::Semaphore& GetRaw();

		std::uint64_t& GetLastValue();

		TimelineSemWrapper() = delete;
		TimelineSemWrapper(const TimelineSemWrapper&) = delete;
		TimelineSemWrapper& operator=(const TimelineSemWrapper&) = delete;

		TimelineSemWrapper(TimelineSemWrapper&& rhs) = delete;
		TimelineSemWrapper& operator=(TimelineSemWrapper&&) = delete;

		~TimelineSemWrapper() = default;
		TimelineSemWrapper(DeviceManager& device_manager_, uint64_t inital_val = 0u);

	private:
		DeviceManager&        device_manager;
		vk::UniqueSemaphore   timeline_sem;

		std::uint64_t         last_value;


	};
}        // namespace Anni












