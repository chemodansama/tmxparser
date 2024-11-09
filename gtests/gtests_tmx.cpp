#include <sstream>

#include <gtest/gtest.h>

#include "Tmx.h"

TEST(TmxMap, Ctor)
{
    auto map = Tmx::Map::ParseText(R"(
<?xml version="1.0" encoding="UTF-8"?>
<map version="1.0"
    tiledversion="1.1.5"
    orientation="orthogonal"
    renderorder="right-down"
    width="10"
    height="10"
    tilewidth="32"
    tileheight="32"
    nextobjectid="11">
</map>
       )");

    EXPECT_EQ(0, map.GetErrorCode());
    EXPECT_EQ(1.0, map.GetVersion());
    EXPECT_EQ(0, map.GetNumLayers());
    EXPECT_EQ(Tmx::MapOrientation::TMX_MO_ORTHOGONAL, map.GetOrientation());
    EXPECT_EQ(Tmx::MapRenderOrder::TMX_RIGHT_DOWN, map.GetRenderOrder());
    EXPECT_EQ(10, map.GetWidth());
    EXPECT_EQ(10, map.GetHeight());
    EXPECT_EQ(32, map.GetTileWidth());
    EXPECT_EQ(32, map.GetTileHeight());
    EXPECT_EQ(11, map.GetNextObjectId());
}

TEST(TmxMap, FailureToParse)
{
    auto map = Tmx::Map::ParseText(R"(
<?xml version="1.0" encoding="UTF-8"?>
<map 
<map>
       )");

    EXPECT_EQ(Tmx::TMX_PARSING_ERROR, map.GetErrorCode());
    EXPECT_NE(0, map.GetErrorText().size()) << map.GetErrorText();
}

template <typename T>
void testMapProperty(const char *props, T&& test)
{
    std::stringstream ss;
    ss << R"(<?xml version="1.0" encoding="UTF-8"?>)";
    ss << "<map " << props << ">" << "</map>";

    const auto map = Tmx::Map::ParseText(ss.str());

    EXPECT_EQ(0, map.GetErrorCode());
    EXPECT_EQ(0, map.GetNumLayers());

    test(map);
}

TEST(TmxMap, DefaultOrientation)
{
    testMapProperty("", [](const Tmx::Map &map) {
        EXPECT_EQ(Tmx::MapOrientation::TMX_MO_ORTHOGONAL, map.GetOrientation());
    });
}

TEST(TmxMap, OrthogonalOrientation)
{
    testMapProperty(R"(orientation="orthogonal")", [](const Tmx::Map &map) {
        EXPECT_EQ(Tmx::MapOrientation::TMX_MO_ORTHOGONAL, map.GetOrientation());
    });
}

TEST(TmxMap, IsometricOrientation)
{
    testMapProperty(R"(orientation="isometric")", [](const Tmx::Map &map) {
        EXPECT_EQ(Tmx::MapOrientation::TMX_MO_ISOMETRIC, map.GetOrientation());
    });
}

TEST(TmxMap, StaggeredOrientation)
{
    testMapProperty(R"(orientation="staggered")", [](const Tmx::Map &map) {
        EXPECT_EQ(Tmx::MapOrientation::TMX_MO_STAGGERED, map.GetOrientation());
    });
}

TEST(TmxMap, HexagonalOrientation)
{
    testMapProperty(R"(orientation="hexagonal")", [](const Tmx::Map &map) {
        EXPECT_EQ(Tmx::MapOrientation::TMX_MO_HEXAGONAL, map.GetOrientation());
    });
}

TEST(TmxMap, DefaultRenderOrder)
{
    testMapProperty("", [](const Tmx::Map &map) {
        EXPECT_EQ(Tmx::MapRenderOrder::TMX_RIGHT_DOWN, map.GetRenderOrder());
    });
}

TEST(TmxMap, RightDownRenderOrder)
{
    testMapProperty(R"(renderorder="right-down")", [](const Tmx::Map &map) {
        EXPECT_EQ(Tmx::MapRenderOrder::TMX_RIGHT_DOWN, map.GetRenderOrder());
    });
}

TEST(TmxMap, RightUpRenderOrder)
{
    testMapProperty(R"(renderorder="right-up")", [](const Tmx::Map &map) {
        EXPECT_EQ(Tmx::MapRenderOrder::TMX_RIGHT_UP, map.GetRenderOrder());
    });
}

TEST(TmxMap, LeftDownRenderOrder)
{
    testMapProperty(R"(renderorder="left-down")", [](const Tmx::Map &map) {
        EXPECT_EQ(Tmx::MapRenderOrder::TMX_LEFT_DOWN, map.GetRenderOrder());
    });
}

TEST(TmxMap, LeftUpRenderOrder)
{
    testMapProperty(R"(renderorder="left-up")", [](const Tmx::Map &map) {
        EXPECT_EQ(Tmx::MapRenderOrder::TMX_LEFT_UP, map.GetRenderOrder());
    });
}

TEST(TmxMap, DefaultWidth)
{
    testMapProperty("", [](const Tmx::Map &map) {
        EXPECT_EQ(0, map.GetWidth());
    });
}

TEST(TmxMap, Width)
{
    testMapProperty(R"(width="32")", [](const Tmx::Map &map) {
        EXPECT_EQ(32, map.GetWidth());
    });
}

TEST(TmxMap, DefaultHeight)
{
    testMapProperty("", [](const Tmx::Map &map) {
        EXPECT_EQ(0, map.GetHeight());
    });
}

TEST(TmxMap, Height)
{
    testMapProperty(R"(height="32")", [](const Tmx::Map &map) {
        EXPECT_EQ(32, map.GetHeight());
    });
}

TEST(TmxMap, DefaultTileWidth)
{
    testMapProperty("", [](const Tmx::Map &map) {
        EXPECT_EQ(0, map.GetTileWidth());
    });
}

TEST(TmxMap, TileWidth)
{
    testMapProperty(R"(tilewidth="32")", [](const Tmx::Map &map) {
        EXPECT_EQ(32, map.GetTileWidth());
    });
}

TEST(TmxMap, DefaultTileHeight)
{
    testMapProperty("", [](const Tmx::Map &map) {
        EXPECT_EQ(0, map.GetTileHeight());
    });
}

TEST(TmxMap, TileHeight)
{
    testMapProperty(R"(tileheight="32")", [](const Tmx::Map &map) {
        EXPECT_EQ(32, map.GetTileHeight());
    });
}

TEST(TmxMap, DefaultHexSideLength)
{
    testMapProperty("", [](const Tmx::Map &map) {
        EXPECT_EQ(0, map.GetHexsideLength());
    });
}

TEST(TmxMap, HexSideLength)
{
    testMapProperty(R"(hexsidelength="123")", [](const Tmx::Map &map) {
        EXPECT_EQ(123, map.GetHexsideLength());
    });
}

TEST(TmxMap, DefaultStaggerAxis)
{
    testMapProperty(R"()", [](const Tmx::Map &map) {
        EXPECT_EQ(Tmx::MapStaggerAxis::TMX_SA_NONE, map.GetStaggerAxis());
    });
}

TEST(TmxMap, StaggerAxisX)
{
    testMapProperty(R"(staggeraxis="x")", [](const Tmx::Map &map) {
        EXPECT_EQ(Tmx::MapStaggerAxis::TMX_SA_X, map.GetStaggerAxis());
    });
}

TEST(TmxMap, StaggerAxisY)
{
    testMapProperty(R"(staggeraxis="y")", [](const Tmx::Map &map) {
        EXPECT_EQ(Tmx::MapStaggerAxis::TMX_SA_Y, map.GetStaggerAxis());
    });
}

TEST(TmxMap, DefaultStaggerIndex)
{
    testMapProperty(R"()", [](const Tmx::Map &map) {
        EXPECT_EQ(Tmx::MapStaggerIndex::TMX_SI_NONE, map.GetStaggerIndex());
    });
}

TEST(TmxMap, StaggerIndexEven)
{
    testMapProperty(R"(staggerindex="even")", [](const Tmx::Map &map) {
        EXPECT_EQ(Tmx::MapStaggerIndex::TMX_SI_EVEN, map.GetStaggerIndex());
    });
}

TEST(TmxMap, StaggerIndexOdd)
{
    testMapProperty(R"(staggerindex="odd")", [](const Tmx::Map &map) {
        EXPECT_EQ(Tmx::MapStaggerIndex::TMX_SI_ODD, map.GetStaggerIndex());
    });
}

TEST(TmxMap, DefaultParallaxOriginX)
{
    testMapProperty(R"()", [](const Tmx::Map &map) {
        EXPECT_EQ(0.0, map.GetParallaxOriginX());
    });
}

TEST(TmxMap, ParallaxOriginX)
{
    testMapProperty(R"(parallaxoriginx="0.5")", [](const Tmx::Map &map) {
        EXPECT_EQ(0.5, map.GetParallaxOriginX());
    });

    testMapProperty(R"(parallaxoriginx="1.0")", [](const Tmx::Map &map) {
        EXPECT_EQ(1.0, map.GetParallaxOriginX());
    });
}


TEST(TmxMap, DefaultParallaxOriginY)
{
    testMapProperty(R"()", [](const Tmx::Map &map) {
        EXPECT_EQ(0.0, map.GetParallaxOriginY());
    });
}

TEST(TmxMap, ParallaxOriginY)
{
    testMapProperty(R"(parallaxoriginy="0.5")", [](const Tmx::Map &map) {
        EXPECT_EQ(0.5, map.GetParallaxOriginY());
    });

    testMapProperty(R"(parallaxoriginy="1.0")", [](const Tmx::Map &map) {
        EXPECT_EQ(1.0, map.GetParallaxOriginY());
    });
}

TEST(TmxMap, DefaultBackGroundColor)
{
    testMapProperty(R"()", [](const Tmx::Map &map) {
        EXPECT_EQ(Tmx::Color(0, 0, 0, 0), map.GetBackgroundColor());
    });
}

TEST(TmxMap, BackGroundColorNoAlpha)
{
    testMapProperty(R"(backgroundcolor="#7F00FF")", [](const Tmx::Map &map) {
        EXPECT_EQ(Tmx::Color(0x7F, 0, 0xFF, 0xFF), map.GetBackgroundColor());
    });
}

TEST(TmxMap, BackGroundColorWithAlpha)
{
    testMapProperty(R"(backgroundcolor="#34123456")", [](const Tmx::Map &map) {
        EXPECT_EQ(Tmx::Color(0x12, 0x34, 0x56, 0x34), map.GetBackgroundColor());
    });
}

TEST(TmxMap, DefaultInfinite)
{
    testMapProperty(R"()", [](const Tmx::Map &map) {
        EXPECT_EQ(false, map.IsInfinite());
    });
}

TEST(TmxMap, Infinite)
{
    testMapProperty(R"(infinite="0")", [](const Tmx::Map &map) {
        EXPECT_EQ(false, map.IsInfinite());
    });

    testMapProperty(R"(infinite="1")", [](const Tmx::Map &map) {
        EXPECT_EQ(true, map.IsInfinite());
    });
}