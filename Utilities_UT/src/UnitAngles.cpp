#include <gtest/gtest.h>

#include "Units/Angles.h"

using namespace LCN::UnitsAngle::Literals;

namespace LCN::UnitTests
{
    TEST(UnitAngles, AnglesConversion)
    {
        EXPECT_EQ(60 * (45_deg).Count(), (2700_amin).Count());
        EXPECT_EQ(45_deg, 2700_amin);
        EXPECT_EQ(1_amin, 60_asec);
        EXPECT_EQ(1_deg, 3600_asec);
    }

    TEST(UnitAngles, AnglesComparisons)
    {
        EXPECT_TRUE(1.0_rad < 90_deg);
        EXPECT_TRUE(2700_amin <= 45_deg);
        EXPECT_TRUE(180_deg > 3.14_rad);
        EXPECT_TRUE(45_deg >= 2700_amin);

        EXPECT_TRUE(1_deg == 60_amin);
        EXPECT_TRUE(2_deg != 60_amin);

        EXPECT_NEAR(
            (180_deg).Count(),
            (LCN::Units::UnitCast<LCN::UnitsAngle::DegreeLD>(3.14_rad)).Count(),
            (0.5_deg).Count());
    }

    TEST(UnitAngles, AnglesArithmetics)
    {
        const auto add  = 45_deg + 30_amin;
        const auto sub  = 45_deg - 30_amin;
        const auto mul1 = 2 * 45_deg;
        const auto mul2 = 45_deg * 2;
        const auto div  = 90_deg / 2;
        const auto mod  = 370_deg % 360_deg;

        EXPECT_EQ(add,  2730_amin);
        EXPECT_EQ(sub,  2670_amin);
        EXPECT_EQ(mul1, 90_deg);
        EXPECT_EQ(mul2, 90_deg);
        EXPECT_EQ(div,  45_deg);
        EXPECT_EQ(mod,  10_deg);
    }
}