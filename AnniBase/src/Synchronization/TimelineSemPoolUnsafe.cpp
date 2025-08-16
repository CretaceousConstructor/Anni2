#include "Synchronization/TimelineSemPoolUnsafe.h"

namespace Anni2
{

	void TimelineSemPoolUnsafe::TimelineSemDeleter::operator()(TimelineSemWrapper* ptr) const
	{
		pool->ReleaseObject(ptr);
	}

	TimelineSemPoolUnsafe::TimelineSemPoolUnsafe(DeviceManager& device_manager_) :
		device_manager(device_manager_)
	{
	}

	TimelineSemPoolUnsafe::UniqueTimelineSem TimelineSemPoolUnsafe::AcquireTimelineSem()
	{
		if (!pool.empty())
		{
			auto result_timeline_sem = pool.front();
			pool.pop_front();
			return UniqueTimelineSem(result_timeline_sem, TimelineSemDeleter{this});
		}
		else
		{
			auto result_timeline_sem = new TimelineSemWrapper(device_manager);
			return UniqueTimelineSem(result_timeline_sem, TimelineSemDeleter{this});
		}
	}

	void TimelineSemPoolUnsafe::ReleaseObject(TimelineSemWrapper* obj)
	{
		// Optionally reset the object state
		if (pool.size() < max_capacity)
		{
			const uint64_t cur_val = obj->ReturnState();
			obj->GetLastValue() = cur_val;
			pool.push_back(obj);
		}
		else
		{
			delete obj;
		}
	}


}        // namespace Anni
