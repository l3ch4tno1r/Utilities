#include "Benchmarking.h"

namespace LCN
{
	namespace Benchmark
	{
		SessionTimer::SessionTimer(Session& session) :
			m_Session(session),
			m_Start(std::chrono::high_resolution_clock::now())
		{}

		SessionTimer::~SessionTimer()
		{
			auto end = std::chrono::high_resolution_clock::now();

			m_Session.m_SessionDurationNanos = std::chrono::duration_cast<std::chrono::nanoseconds>(end - m_Start).count();
		}

		long long Session::Duration(DurationUnit unit)
		{
			return m_SessionDurationNanos / unit;
		}
	}
}
