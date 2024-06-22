#include <gtest/gtest.h>

#include "Utilities/Arguments.h"

namespace LCN::UnitTests
{
    TEST(Arguments, Nominal)
    {
        const char* argv[] = {
            "-param1", "1",
            "-param2", "2"
        };

        LCN::Utilities::Arguments args{ std::size(argv), argv };

        ASSERT_EQ(args.size(), 4);

        EXPECT_EQ(args[0], "-param1");
        EXPECT_EQ(args[1], "1");
        EXPECT_EQ(args[2], "-param2");
        EXPECT_EQ(args[3], "2");

        auto argRange = args
            | std::ranges::views::filter([](const auto arg)
            {
                return not arg.empty() and arg[0] == '-';
            });
        
        auto it = argRange.begin(), end = argRange.end();

        ASSERT_NE(it, end); EXPECT_EQ(*(it++), "-param1");
        ASSERT_NE(it, end); EXPECT_EQ(*(it++), "-param2");
    }
}