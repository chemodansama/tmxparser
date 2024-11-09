#include <sstream>

#include <gtest/gtest.h>

#include "Tmx.h"

TEST(TmxTileset, Ctor)
{
    tinyxml2::XMLDocument d;
    d.Parse(R"(
<tileset version="1.8" tiledversion="1.8.6" name="globalmap_background"
    tilewidth="128" tileheight="256" tilecount="2" columns="7" spacing="123"
    margin="87" >

    <grid orientation="orthogonal" width="1" height="1" />
    <tile id="0">
        <image width="2048" height="2048" source="tiles/xhd/pack_0_0.png" />
    </tile>
    <tile id="1">
        <image width="2048" height="2048" source="tiles/xhd/pack_0_1.png" />
    </tile>
</tileset>
)");

    Tmx::Tileset t{ "", d.RootElement() };
    EXPECT_EQ(0, t.GetFirstGid());
    EXPECT_EQ("globalmap_background", t.GetName());
    EXPECT_EQ(128, t.GetTileWidth());
    EXPECT_EQ(256, t.GetTileHeight());

    EXPECT_EQ(123, t.GetSpacing());
    EXPECT_EQ(87, t.GetMargin());

    EXPECT_EQ(2, t.GetTileCount());
    EXPECT_EQ(7, t.GetColumns());
}