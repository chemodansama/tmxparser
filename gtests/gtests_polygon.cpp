#include <sstream>

#include <gtest/gtest.h>

#include "Tmx.h"

TEST(TmxPolygon, EmptyStringCtor)
{
    Tmx::Polygon p{ "" };
    ASSERT_EQ(0, p.GetNumPoints());
}

TEST(TmxPolygon, NullDataCtor)
{
    Tmx::Polygon p{ nullptr };
    ASSERT_EQ(0, p.GetNumPoints());
}

TEST(TmxPolygon, Ctor)
{
    Tmx::Polygon p{ "1,2 3.5,4 5.25,6" };
    ASSERT_EQ(3, p.GetNumPoints());
    ASSERT_EQ(Tmx::Point(1.0f, 2.0f), p.GetPoint(0));
    ASSERT_EQ(Tmx::Point(3.5f, 4.0f), p.GetPoint(1));
    ASSERT_EQ(Tmx::Point(5.25f, 6.0f), p.GetPoint(2));
}

TEST(TmxPolygon, OnePointCtor)
{
    Tmx::Polygon p{ "1.25,2.75" };
    ASSERT_EQ(1, p.GetNumPoints());
    ASSERT_EQ(Tmx::Point(1.25f, 2.75f), p.GetPoint(0));
}
