#include "Utilities/Profiler.h"

namespace LCN::Utilities
{
#pragma region Profiler

    //////////////////
    //-- Profiler --//
    //////////////////

    ScopeTimer
    Profiler::CreateTimer(
        const std::string_view name)
    {
        return { name };
    }

#pragma endregion

    ////////////////////
    //-- ScopeTimer --//
    ////////////////////

    ScopeTimer::ScopeTimer(const std::string_view name)
        : m_name{ name }
        , m_start{ std::chrono::high_resolution_clock::now() }
    {}
}