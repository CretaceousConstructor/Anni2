#pragma once

#include "Synchronization/TimelineSemWrapper.h"
#include <deque>

namespace Anni2
{
	class TimelineSemPoolUnsafe
	{
		class TimelineSemDeleter 
		{
			friend class TimelineSemPoolUnsafe;

			TimelineSemDeleter(TimelineSemPoolUnsafe* pool_) :
				pool(pool_)
			{

			}

			TimelineSemPoolUnsafe* pool;  // 必须通过构造函数传入 pool
		public:
			void operator()(TimelineSemWrapper* ptr) const;
		};

	public:
		using UniqueTimelineSem = std::unique_ptr<TimelineSemWrapper, TimelineSemDeleter>;

	public:
		static constexpr size_t max_capacity{ 64 };
		explicit TimelineSemPoolUnsafe(DeviceManager& device_manager_);
		UniqueTimelineSem                   AcquireTimelineSem();
	private:
		void                                ReleaseObject(TimelineSemWrapper* obj);
	private:
		std::deque<TimelineSemWrapper*> pool;
	private:
		DeviceManager& device_manager;
	};

}        // namespace Anni
