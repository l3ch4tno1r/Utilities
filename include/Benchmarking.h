#pragma once

#include <future>
#include <chrono>

namespace LCN::Benchmark
{
	enum DurationUnit
	{
		Seconds      = 1000000000,
		Milliseconds = 1000000,
		Microseconds = 1000,
		Nanoseconds  = 1
	};

	class Session
	{
	public:
		long long Duration(DurationUnit unit) const;

	private:
		long long m_SessionDurationNanos;

		friend class SessionTimer;
	};

	class SessionTimer
	{
	public:
		SessionTimer(Session& session);

		~SessionTimer();

	private:
		Session& m_Session;

		std::chrono::steady_clock::time_point m_Start;
	};

	template<class Function, typename ...Args>
	Session TimePerformance(size_t iterations, Function f, Args&&... args)
	{
		Session session;

		{
			SessionTimer timer(session);
	
			for (size_t i = 0; i < iterations; ++i)
				f(std::forward<Args>(args)...);
		}

		return session;
	}

	template<class Function, typename ...Args>
	std::future<Session> TimePerformanceAsync(size_t iterations, Function f, Args&&... args)
	{
		return std::async(
			std::launch::async,
			TimePerformance<Function, Args...>, iterations,
			f,
			std::forward<Args>(args)...);
	}
}