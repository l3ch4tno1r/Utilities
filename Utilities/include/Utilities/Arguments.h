#pragma once

#include <string_view>
#include <span>
#include <ranges>

namespace LCN::Utilities
{
    #define _CharType char

    namespace Details
    {
        inline
        std::basic_string_view<_CharType>
        ConstCharPtrToStringView(
            const _CharType* raw)
        {
            return { raw };
        }

        inline
        auto
        CreateArgRange(
            const int argc,
            const _CharType** argv)
        {
            return std::span{ argv, (size_t)argc }
                | std::ranges::views::transform(ConstCharPtrToStringView);
        }

        using ArgRangeType = decltype(CreateArgRange(0, nullptr));
    }

    ///////////////////
    //-- Arguments --//
    ///////////////////

    struct Arguments : Details::ArgRangeType
    {
        Arguments(const int argc, _CharType** argv)
            : Details::ArgRangeType{ Details::CreateArgRange(argc, const_cast<const _CharType**>(argv)) }
        {}

        Arguments(const int argc, const _CharType** argv)
            : Details::ArgRangeType{ Details::CreateArgRange(argc, argv) }
        {}
    };
}