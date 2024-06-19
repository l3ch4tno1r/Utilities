#pragma once

#include <chrono>
#include <filesystem>

namespace fs = std::filesystem;

#ifdef PROFILING
#define TOKENPASTE(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)

/**
 * @brief Convenience macro that automatically creates a scope timer
 * @param name Name of the scope
*/
#define PROFILE_SCOPE(name) auto TOKENPASTE2(timer, __LINE__) = LCN::Utilities::Profiler::CreateTimer(name)

/**
 * @brief Convenience macro that automatically creates a timer with the signature of the function to be profiled
*/
#define PROFILE_FUNC() PROFILE_SCOPE(__FUNCSIG__)
#else
#define PROFILE_SCOPE(name)
#define PROFILE_FUNC()
#endif

namespace LCN::Utilities
{
    class ScopeTimer;

    //////////////////
    //-- Profiler --//
    //////////////////

    class Profiler
    {
    public:

        /**
         * @brief Creates a profiling timer 
         * @param name Name of the timer
         * @return ScopeTimer
        */
        static ScopeTimer CreateTimer(const std::string_view);

    private:
        fs::path m_outputPath;
    };

    ////////////////////
    //-- ScopeTimer --//
    ////////////////////

    class ScopeTimer
    {
    private:
        ScopeTimer(const std::string_view);

        friend Profiler;

    private:
        const std::string_view m_name;
        const std::chrono::high_resolution_clock::time_point m_start;
    };
}