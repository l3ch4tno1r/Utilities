#pragma once

#include <future>

#include "TimeMeasurement.h"

namespace Benchmark
{
	template<class Function, typename ...Args>
	void TimePerformance(size_t iterations, Function f, Args&&... args)
	{
		Timer timer;
	
		for (size_t i = 0; i < iterations; ++i)
			f(std::forward<Args>(args)...);
	}

	template<class Function, typename ...Args>
	std::future<void> TimePerformanceAsync(size_t iterations, Function f, Args&&... args)
	{
		return std::async(std::launch::async, TimePerformance<Function, Args...>, iterations, f, std::forward<Args>(args)...);
	}
}