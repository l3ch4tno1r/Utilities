#include <ranges>

#include <gtest/gtest.h>

#include "Utilities/SplitString.h"

namespace LCN::UnitTests
{
    TEST(SplitString, Nominal)
    {
        constexpr std::string_view input{ "Hello world !" };
        auto split = LCN::Split(input, " ");

        ASSERT_EQ(3, split.Count());

        auto it = split.begin();

        static_assert(std::ranges::range<decltype(split)>);

        EXPECT_EQ(*(it++), "Hello");
        EXPECT_EQ(*(it++), "world");
        EXPECT_EQ(*(it++), "!");

        ASSERT_EQ(it, split.end());
    }

    TEST(SplitString, Iterating)
    {
        constexpr std::string_view csv{ "Joe;Jack;William;Averell" };
        auto split = LCN::Split(csv, ";");

        constexpr std::string_view expected[] = {
            "Joe",
            "Jack",
            "William",
            "Averell"
        };

        auto it = std::begin(expected);

        for(const auto sv : split)
            EXPECT_EQ(*(it++), sv);
    }

    //TEST(SplitString, Range)
    //{
    //    constexpr std::string_view csv{ "1;2;3;4;5;6" };
    //
    //    auto split = LCN::Split(csv, ";")
    //        | std::ranges::views::take(3);
    //}
}