#include <filesystem>

#include <gtest/gtest.h>

#include "Utilities/Arguments.h"

namespace fs = std::filesystem;

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

        ASSERT_EQ(it, end);
    }

    TEST(Arguments, ValueExtractionNominal)
    {
        const char* argv[] = {
            "-a",  "12",
            "-b",  "-4",
            "-f", "1.3",
            "-i",  "26",
            "-path",  "/home/user/Documents"
        };

        LCN::Utilities::Arguments args{ std::size(argv), argv };

        ASSERT_EQ(args.size(), std::size(argv));

        fs::path path;
        args["-path"] >> path;

        EXPECT_EQ(path, "/home/user/Documents");

        int a, b;
        float f;
        unsigned int i;

        EXPECT_EQ(std::errc{}, args["-a"] >> a);
        EXPECT_EQ(std::errc{}, args["-b"] >> b);
        EXPECT_EQ(std::errc{}, args["-f"] >> f);
        EXPECT_EQ(std::errc{}, args["-i"] >> i);

        EXPECT_EQ(a, 12);
        EXPECT_EQ(b, -4);
        EXPECT_EQ(f, 1.3f);
        EXPECT_EQ(i, 26);
    }

    TEST(Arguments, ValueExtractionBadData)
    {
        const char* argv[] = {
            "-a", "Hello world !",
            "-b", "1000000000000000",
            "-c"
        };

        LCN::Utilities::Arguments args{ std::size(argv), argv };

        int a, b, c;

        EXPECT_EQ(std::errc::invalid_argument,    args["-a"] >> a);
        EXPECT_EQ(std::errc::result_out_of_range, args["-b"] >> b);
        EXPECT_EQ(std::errc::invalid_argument,    args["-c"] >> c);

        std::string str;
        args["-c"] >> str;

        EXPECT_EQ(str, "");
    }
}