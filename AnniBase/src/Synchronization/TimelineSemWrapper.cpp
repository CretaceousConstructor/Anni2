#include "Synchronization/TimelineSemWrapper.h"

namespace Anni2
{
	uint64_t TimelineSemWrapper::ReturnState()
	{
		return device_manager.GetLogicalDevice().getSemaphoreCounterValue(timeline_sem.get()).value;
	}

	vk::Semaphore& TimelineSemWrapper::GetRaw()
	{
		return *timeline_sem;
	}

	std::uint64_t& TimelineSemWrapper::GetLastValue()
	{
		return last_value;
	}

	TimelineSemWrapper::TimelineSemWrapper(DeviceManager& device_manager_, uint64_t inital_val) :
		device_manager(device_manager_)
		,last_value(inital_val)
	{
		vk::SemaphoreTypeCreateInfo timeline_CI
		(
			vk::SemaphoreType::eTimeline,
			inital_val
		);

		vk::SemaphoreCreateInfo sem_CI(vk::SemaphoreCreateFlags(VK_ZERO_FLAG), &timeline_CI);
		timeline_sem = device_manager.GetLogicalDevice().createSemaphoreUnique(sem_CI).value;
	}

}


