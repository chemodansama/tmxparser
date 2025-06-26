//-----------------------------------------------------------------------------
// TmxMap.cpp
//
// Copyright (c) 2010-2014, Tamir Atias
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL TAMIR ATIAS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Author: Tamir Atias
//-----------------------------------------------------------------------------

#include "TmxMap.h"

#include <cassert>

#include <tinyxml2.h>

#include "TmxGroupLayer.h"
#include "TmxImageLayer.h"
#include "TmxLayer.h"
#include "TmxObjectGroup.h"
#include "TmxTileLayer.h"
#include "TmxTileset.h"
#include "TmxUtil.h"

namespace Tmx
{
    namespace
    {
        auto GetFilePath(const std::string &fileName)
        {
            const int lastSlash = fileName.find_last_of("/");

            // Get the directory of the file using substring.
            return lastSlash > 0
                ? fileName.substr(0, lastSlash + 1)
                : std::string{};
        }

        auto GetMapElement(const tinyxml2::XMLDocument *doc)
        {
            return doc->FirstChildElement("map");
        }

        auto GetStringAttribute(const tinyxml2::XMLElement *data, const char *attribute)
        {
            if (const auto s = data->Attribute(attribute))
            {
                return std::string_view{ s };
            }

            return std::string_view{};
        }

        auto ParseOrientation(const tinyxml2::XMLElement *data)
        {
            const auto attribute = GetStringAttribute(data, "orientation");

            return
                attribute == "orthogonal" ? TMX_MO_ORTHOGONAL :
                attribute == "isometric"  ? TMX_MO_ISOMETRIC :
                attribute == "staggered"  ? TMX_MO_STAGGERED :
                attribute == "hexagonal"  ? TMX_MO_HEXAGONAL
                                          : TMX_MO_ORTHOGONAL;
        }

        auto ParseRenderOrder(const tinyxml2::XMLElement *data)
        {
            const auto attribute = GetStringAttribute(data, "renderorder");

            return
                attribute == "right-down" ? TMX_RIGHT_DOWN :
                attribute == "right-up"   ? TMX_RIGHT_UP :
                attribute == "left-down"  ? TMX_LEFT_DOWN :
                attribute == "left-up"    ? TMX_LEFT_UP
                                          : TMX_RIGHT_DOWN;
        }

        auto ParseStaggerAxis(const tinyxml2::XMLElement *data)
        {
            const auto attribute = GetStringAttribute(data, "staggeraxis");

            return
                attribute == "x" ? TMX_SA_X :
                attribute == "y" ? TMX_SA_Y
                                 : TMX_SA_NONE;
        }

        auto ParseStaggerIndex(const tinyxml2::XMLElement *data)
        {
            const auto attribute = GetStringAttribute(data, "staggerindex");

            return
                attribute == "even" ? TMX_SI_EVEN :
                attribute == "odd"  ? TMX_SI_ODD
                                    : TMX_SI_NONE;
        }
    }

    Map Map::ParseFile(const std::string &fileName)
    {
        tinyxml2::XMLDocument doc;
        doc.LoadFile(fileName.c_str());

        return doc.Error()
            ? Map{ doc.ErrorStr() }
            : Map{ GetMapElement(&doc), GetFilePath(fileName)};
    }

    Map Map::ParseText(const std::string &text, const std::string &path)
    {
        return ParseText(text.c_str(), path);
    }

    Map Map::ParseText(const char *text, const std::string &path)
    {
        // Create a tiny xml document and use it to parse the text.
        tinyxml2::XMLDocument doc;
        doc.Parse(text);

        return doc.Error()
            ? Map{ doc.ErrorStr() }
            : Map{ GetMapElement(&doc), path };
    }

    const Tmx::Layer *Map::GetLayer(int index) const
    {
        return layers.at(index);
    }

    int Map::GetNumLayers() const
    {
        return layers.size();
    }

    const Tmx::TileLayer *Map::GetTileLayer(int index) const
    {
        return &tile_layers.at(index);
    }

    int Map::GetNumTileLayers() const
    {
        return tile_layers.size();
    }

    const Tmx::ObjectGroup *Map::GetObjectGroup(int index) const
    {
        return &object_groups.at(index);
    }

    int Map::GetNumObjectGroups() const
    {
        return object_groups.size();
    }

    const std::vector<Tmx::ObjectGroup> &Map::GetObjectGroups() const
    {
        return object_groups;
    }

    const Tmx::ImageLayer *Map::GetImageLayer(int index) const
    {
        return &image_layers.at(index);
    }

    int Map::GetNumImageLayers() const
    {
        return image_layers.size();
    }

    const Tmx::GroupLayer *Map::GetGroupLayer(int index) const
    {
        return &group_layers.at(index);
    }

    int Map::GetNumGroupLayers() const
    {
        return group_layers.size();
    }

    int Map::FindTilesetIndex(int gid) const
    {
        // Clean up the flags from the gid (thanks marwes91).
        gid &= ~(FlippedHorizontallyFlag | FlippedVerticallyFlag | FlippedDiagonallyFlag);

        for (int i = tilesets.size() - 1; i > -1; --i)
        {
            // If the gid beyond the tileset gid return its index.
            if (gid >= tilesets[i].GetFirstGid())
            {
                return i;
            }
        }

        return -1;
    }

    const Tileset *Map::FindTileset(int gid) const
    {
        for (int i = tilesets.size() - 1; i > -1; --i)
        {
            // If the gid beyond the tileset gid return it.
            if (gid >= tilesets[i].GetFirstGid())
            {
                return &tilesets[i];
            }
        }

        return nullptr;
    }

    const Tmx::Tileset *Map::GetTileset(int index) const
    {
        return &tilesets.at(index);
    }

    int Map::GetNumTilesets() const
    {
        return tilesets.size();
    }

    Map::Map(std::string errorText)
        : has_error{ true }
        , error_code{ TMX_PARSING_ERROR }
        , error_text{ std::move(errorText) }
        , properties{ nullptr }
    {
    }

    Map::Map(const tinyxml2::XMLElement *data, std::string filePath)
        : file_path{ std::move(filePath) }
        , background_color{ Util::ParseOrDefault(data, "backgroundcolor",
            [](const auto s) { return Tmx::Color{ s }; }, {}) }
        , version{ data->DoubleAttribute("version") }
        , orientation{ ParseOrientation(data) }
        , render_order{ ParseRenderOrder(data) }
        , stagger_axis{ ParseStaggerAxis(data) }
        , stagger_index{ ParseStaggerIndex(data) }
        , width{ data->IntAttribute("width") }
        , height{ data->IntAttribute("height") }
        , tile_width{ data->IntAttribute("tilewidth") }
        , tile_height{ data->IntAttribute("tileheight") }
        , next_object_id{ data->IntAttribute("nextobjectid") }
        , hexside_length{ data->IntAttribute("hexsidelength") }
        , parallaxOriginX{ data->FloatAttribute("parallaxoriginx") }
        , parallaxOriginY{ data->FloatAttribute("parallaxoriginy") }
        , infinite{ static_cast<bool>(data->IntAttribute("infinite")) }
        , properties{ data->FirstChildElement("properties") }
    {
        // read all other attributes
        const auto *node = data->FirstChild();
        while (node) {
            // Iterate through all of the "layer" (tile layer) elements.
            const auto v = node->Value();
            if (!v) {
                continue;
            }

            const auto element = node->ToElement();

            if (strcmp(node->Value(), "tileset") == 0) {
                tilesets.emplace_back(file_path, element);
            }

            if (strcmp(node->Value(), "layer") == 0) {
                tile_layers.emplace_back(this, element);
            }

            if (strcmp(node->Value(), "imagelayer") == 0) {
                image_layers.emplace_back(this, element);
            }

            if (strcmp(node->Value(), "objectgroup") == 0) {
                object_groups.emplace_back(this, element);
            }

            if (strcmp(node->Value(), "group") == 0) {
                group_layers.emplace_back(this, element);
            }

            node = node->NextSibling();
        }

        const auto addLayers = [this](auto &collection) {
            for (auto &v : collection) {
                layers.push_back(&v);
            }
        };

        addLayers(tile_layers);
        addLayers(image_layers);
        addLayers(object_groups);
        addLayers(group_layers);
    }
}
