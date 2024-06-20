#include <ranges>
#include <charconv>

#include <gtest/gtest.h>

#include "Utilities/SplitString.h"

namespace LCN::UnitTests
{
    TEST(SplitString, Nominal)
    {
        constexpr std::string_view input{ "Hello world !" };
        auto tokens = LCN::Split(input, " ");

        ASSERT_EQ(tokens.Count(), 3);

        auto it = tokens.begin();

        EXPECT_EQ(*(it++), "Hello");
        EXPECT_EQ(*(it++), "world");
        EXPECT_EQ(*(it++), "!");

        ASSERT_EQ(it, tokens.end());
    }

    TEST(SplitString, EmptyString)
    {
        constexpr std::string_view input{ "" };
        auto tokens = LCN::Split(input, ";");
        
        // Might be controversial, but one can consider that an empty string contains only one token that is empty.
        ASSERT_EQ(tokens.Count(), 1);
    }

    TEST(SplitString, NothingToSplit)
    {
        constexpr std::string_view str{ "Nothing to split here !" };
        auto tokens = LCN::Split(str, ";");

        ASSERT_EQ(tokens.Count(), 1);
        
        EXPECT_EQ(*(tokens.begin()), str);
    }

    TEST(SplitString, Iterating)
    {
        constexpr std::string_view csv{ "Joe;Jack;William;Averell" };
        auto tokens = LCN::Split(csv, ";");

        constexpr std::string_view expected[] = {
            "Joe",
            "Jack",
            "William",
            "Averell"
        };

        ASSERT_EQ(tokens.Count(), 4);

        auto it = std::begin(expected);

        for(const auto sv : tokens)
            EXPECT_EQ(*(it++), sv);
    }

    TEST(SplitString, Range)
    {
        constexpr std::string_view csv{ "1;2;3;4;5;6" };
        auto tokens = LCN::Split(csv, ";");

        static_assert(std::input_or_output_iterator<decltype(tokens)::iterator>);
        static_assert(std::input_iterator<decltype(tokens)::iterator>);
        static_assert(std::forward_iterator<decltype(tokens)::iterator>);
        
        static_assert(std::ranges::range<decltype(tokens)>);
        static_assert(std::ranges::view<decltype(tokens)>);
        static_assert(std::ranges::viewable_range<decltype(tokens)>);
    
        auto tokensRange = tokens
            | std::ranges::views::take(3);
        
        auto it  = tokensRange.begin();
        auto end = tokensRange.end();

        ASSERT_NE(it, end);

        EXPECT_EQ(*(it++), "1");
        EXPECT_EQ(*(it++), "2");
        EXPECT_EQ(*(it++), "3");

        EXPECT_EQ(it, end);
    }

    TEST(SplitString, CSVToNumbers)
    {
        constexpr std::string_view csv{ "1;2;;3;4;;;5;"};
        
        auto tokens = LCN::Split(csv, ";")
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

        for(const int i : tokens)
            EXPECT_EQ(i, expected++);
    }

    TEST(SplitString, ConversionToVector)
    {
        constexpr std::string_view data{ "Joe;Jack;;William;;;Averell;" };
        auto tokens = LCN::Split(data, ";");
        
        ASSERT_EQ(tokens.Count(), 8);        

        auto tokenRange = tokens
            | std::ranges::views::filter([](const auto sv)
            {
                return not sv.empty();
            });
        
        std::vector<std::string> result{ tokenRange.begin(), tokenRange.end() };

        ASSERT_EQ(result.size(), 4);
        
        EXPECT_EQ(result[0], "Joe");
        EXPECT_EQ(result[1], "Jack");
        EXPECT_EQ(result[2], "William");
        EXPECT_EQ(result[3], "Averell");
    }

    TEST(SplitString, WideStrings)
    {
        constexpr std::basic_string_view data{ L"Hello world !" };
        auto tokens = LCN::Split(data, L" ");

        ASSERT_EQ(tokens.Count(), 3);

        auto it = tokens.begin();

        EXPECT_EQ(*(it++), L"Hello");
        EXPECT_EQ(*(it++), L"world");
        EXPECT_EQ(*(it++), L"!");

        EXPECT_EQ(it, tokens.end());
    }
}