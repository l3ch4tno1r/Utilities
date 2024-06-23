#include <gtest/gtest.h>

#include "Units/Angles.h"

using namespace LCN::UnitsAngle::Literals;

namespace LCN::UnitTests
{
    TEST(Units, AnglesConversion)
    {
        EXPECT_EQ(60 * (45_deg).Count(), (2700_amin).Count());
        EXPECT_EQ(45_deg, 2700_amin);
        EXPECT_EQ(1_amin, 60_asec);
    }

    TEST(Units, AnglesArithmetics)
    {}
}