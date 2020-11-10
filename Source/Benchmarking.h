#pragma once

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
}