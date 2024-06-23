#include <gtest/gtest.h>

#include <Utilities/BitwiseEditor.h>

namespace LCN::UnitTests
{
    TEST(Bitwise, Reading)
    {
        int data = 0b101101;
        LCN::Bitwise reader{ std::as_const(data) };

        EXPECT_EQ(reader[0], 1);
        EXPECT_EQ(reader[1], 0);
        EXPECT_EQ(reader[2], 1);
        EXPECT_EQ(reader[3], 1);
        EXPECT_EQ(reader[4], 0);
        EXPECT_EQ(reader[5], 1);
    }

    TEST(Bitwise, Writing)
    {
        int data = 0;
        LCN::Bitwise editor{ data };

        editor[0] = 0;
        editor[1] = 1;
        editor[2] = 0;
        editor[3] = 1;
        editor[4] = 1;

        EXPECT_EQ(data, 0b11010);
    }
}