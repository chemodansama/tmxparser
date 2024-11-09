#include <sstream>
#include <type_traits>

#include <gtest/gtest.h>

#include "Tmx.h"

TEST(TmxProperty, Ctor)
{
    tinyxml2::XMLDocument d;
    d.Parse(R"(
<data>
    <property value="test"/>
</data>
)");

    Tmx::Property p{ d.RootElement()->FirstChildElement("property")};
    EXPECT_EQ(Tmx::TMX_PROPERTY_STRING, p.GetType());
    EXPECT_TRUE(p.IsOfType(Tmx::TMX_PROPERTY_STRING));
    EXPECT_EQ("test", p.GetValue());
}

TEST(TmxProperty, StringType)
{
    tinyxml2::XMLDocument d;
    d.Parse(R"(
<data>
    <property type="string" value="hello"/>
</data>
)");

    Tmx::Property p{ d.RootElement()->FirstChildElement("property") };
    EXPECT_EQ(Tmx::TMX_PROPERTY_STRING, p.GetType());
    EXPECT_TRUE(p.IsOfType(Tmx::TMX_PROPERTY_STRING));
    EXPECT_EQ("hello", p.GetValue());
}

TEST(TmxProperty, BoolTypeTrue)
{
    tinyxml2::XMLDocument d;
    d.Parse(R"(
<data>
    <property type="bool" value="true"/>
</data>
)");

    Tmx::Property p{ d.RootElement()->FirstChildElement("property") };
    EXPECT_EQ(Tmx::TMX_PROPERTY_BOOL, p.GetType());
    EXPECT_TRUE(p.IsOfType(Tmx::TMX_PROPERTY_BOOL));
    EXPECT_EQ(true, p.GetBoolValue());
}

TEST(TmxProperty, BoolTypeFalse)
{
    tinyxml2::XMLDocument d;
    d.Parse(R"(
<data>
    <property type="bool" value="false"/>
</data>
)");

    Tmx::Property p{ d.RootElement()->FirstChildElement("property") };
    EXPECT_EQ(Tmx::TMX_PROPERTY_BOOL, p.GetType());
    EXPECT_TRUE(p.IsOfType(Tmx::TMX_PROPERTY_BOOL));
    EXPECT_EQ(false, p.GetBoolValue());
}

TEST(TmxProperty, BoolTypeWrong)
{
    tinyxml2::XMLDocument d;
    d.Parse(R"(
<data>
    <property type="bool" value="?"/>
</data>
)");

    const auto type = Tmx::TMX_PROPERTY_BOOL;

    Tmx::Property p{ d.RootElement()->FirstChildElement("property") };
    EXPECT_EQ(type, p.GetType());
    EXPECT_TRUE(p.IsOfType(type));
    EXPECT_EQ(false, p.GetBoolValue());
}

TEST(TmxProperty, IntType)
{
    tinyxml2::XMLDocument d;
    d.Parse(R"(
<data>
    <property type="int" value="873"/>
</data>
)");

    const auto type = Tmx::TMX_PROPERTY_INT;

    Tmx::Property p{ d.RootElement()->FirstChildElement("property") };
    EXPECT_EQ(type, p.GetType());
    EXPECT_TRUE(p.IsOfType(type));
    EXPECT_EQ(873, p.GetIntValue());
}

TEST(TmxProperty, FloatType)
{
    tinyxml2::XMLDocument d;
    d.Parse(R"(
<data>
    <property type="float" value="3.0"/>
</data>
)");

    const auto type = Tmx::TMX_PROPERTY_FLOAT;

    Tmx::Property p{ d.RootElement()->FirstChildElement("property") };
    EXPECT_EQ(type, p.GetType());
    EXPECT_TRUE(p.IsOfType(type));
    EXPECT_EQ(3.0f, p.GetFloatValue());
}

TEST(TmxProperty, ColorType)
{
    tinyxml2::XMLDocument d;
    d.Parse(R"(
<data>
    <property type="color" value="05FF7f1f"/>
</data>
)");

    const auto type = Tmx::TMX_PROPERTY_COLOR;

    Tmx::Property p{ d.RootElement()->FirstChildElement("property") };
    EXPECT_EQ(type, p.GetType());
    EXPECT_TRUE(p.IsOfType(type));
    EXPECT_EQ(Tmx::Color(0xFF, 0x7F, 0x1F, 0x05), p.GetColorValue());
}

TEST(TmxProperty, FileType)
{
    tinyxml2::XMLDocument d;
    d.Parse(R"(
<data>
    <property type="file" value="hello.world"/>
</data>
)");

    const auto type = Tmx::TMX_PROPERTY_FILE;

    Tmx::Property p{ d.RootElement()->FirstChildElement("property") };
    EXPECT_EQ(type, p.GetType());
    EXPECT_TRUE(p.IsOfType(type));
    EXPECT_EQ("hello.world", p.GetValue());
}

TEST(TmxProperty, ObjectType)
{
    tinyxml2::XMLDocument d;
    d.Parse(R"(
<data>
    <property type="object" value="123"/>
</data>
)");

    const auto type = Tmx::TMX_PROPERTY_OBJECT;

    Tmx::Property p{ d.RootElement()->FirstChildElement("property") };
    EXPECT_EQ(type, p.GetType());
    EXPECT_TRUE(p.IsOfType(type));
    EXPECT_EQ(123, p.GetIntValue());
}

TEST(TmxProperty, ClassType)
{
    tinyxml2::XMLDocument d;
    d.Parse(R"(
<data>
    <property type="class">
        <properties>
            <property type="int" value="123"/>
            <property type="string" value="hello"/>
        </properties>
    </property>
</data>
)");

    const auto type = Tmx::TMX_PROPERTY_CLASS;

    Tmx::Property p{ d.RootElement()->FirstChildElement("property") };
    EXPECT_EQ(type, p.GetType());
    EXPECT_TRUE(p.IsOfType(type));
}
