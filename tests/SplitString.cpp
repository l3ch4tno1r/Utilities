#include <ranges>
#include <charconv>

#include <gtest/gtest.h>

#include "Utilities/SplitString.h"

namespace LCN::UnitTests
{
    TEST(SplitString, Nominal)
    {
        constexpr std::string_view input{ "Hello world !" };
        auto split = LCN::Split(input, " ");

        ASSERT_EQ(split.Count(), 3);

        auto it = split.begin();

        EXPECT_EQ(*(it++), "Hello");
        EXPECT_EQ(*(it++), "world");
        EXPECT_EQ(*(it++), "!");

        ASSERT_EQ(it, split.end());
    }

    TEST(SplitString, NothingToSplit)
    {
        constexpr std::string_view str{ "Nothing to split here !" };
        auto split = LCN::Split(str, ";");

        EXPECT_EQ(*(split.begin()), str);
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

        ASSERT_EQ(split.Count(), 4);

        auto it = std::begin(expected);

        for(const auto sv : split)
            EXPECT_EQ(*(it++), sv);
    }

    TEST(SplitString, Range)
    {
        constexpr std::string_view csv{ "1;2;3;4;5;6" };
        auto split = LCN::Split(csv, ";");

        static_assert(std::input_or_output_iterator<decltype(split)::iterator>);
        static_assert(std::input_iterator<decltype(split)::iterator>);
        static_assert(std::forward_iterator<decltype(split)::iterator>);
        
        static_assert(std::ranges::range<decltype(split)>);
        static_assert(std::ranges::view<decltype(split)>);
        static_assert(std::ranges::viewable_range<decltype(split)>);
    
        auto splitRange = split
            | std::ranges::views::take(3);
        
        auto it  = splitRange.begin();
        auto end = splitRange.end();

        ASSERT_NE(it, end);

        EXPECT_EQ(*(it++), "1");
        EXPECT_EQ(*(it++), "2");
        EXPECT_EQ(*(it++), "3");

        EXPECT_EQ(it, end);
    }

    TEST(SplitString, CSVToNumbers)
    {
        constexpr std::string_view csv{ "1;2;;3;4;;;5;"};
        
        auto split = LCN::Split(csv, ";")
            | std::ranges::views::transform([](const std::string_view sv) -> std::optional<int>
            {
                int value;

                auto conv = std::from_chars(
                    sv.data(),
                    sv.data() + sv.size(),
                    value);

                return conv.ec == std::errc{}
                    ? std::make_optional(value)
                    : std::nullopt;
            })
            | std::ranges::views::filter([](const std::optional<int>& opt){ return opt.has_value(); })
            | std::ranges::views::transform([](const auto& opt){ return *opt; });
        
        int expected = 1;

        for(const int i : split)
            EXPECT_EQ(i, expected++);
    }

    TEST(SplitString, ConversionToVector)
    {
        constexpr std::string_view data{ "Joe;Jack;;William;;;Averell;" };

        auto split = LCN::Split(data, ";")
            | std::ranges::views::filter([](const auto sv)
            {
                return not sv.empty();
            });
        
        std::vector<std::string> tokens{ split.begin(), split.end() };

        ASSERT_EQ(tokens.size(), 4);
        
        EXPECT_EQ(tokens[0], "Joe");
        EXPECT_EQ(tokens[1], "Jack");
        EXPECT_EQ(tokens[2], "William");
        EXPECT_EQ(tokens[3], "Averell");
    }

    TEST(SplitString, WideStrings)
    {
        constexpr std::basic_string_view data{ L"Hello world !" };
        auto split = LCN::Split(data, L" ");

        ASSERT_EQ(split.Count(), 3);

        auto it = split.begin();

        EXPECT_EQ(*(it++), L"Hello");
        EXPECT_EQ(*(it++), L"world");
        EXPECT_EQ(*(it++), L"!");

        EXPECT_EQ(it, split.end());
    }
}