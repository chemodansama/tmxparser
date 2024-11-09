//-----------------------------------------------------------------------------
// TmxTileset.cpp
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

#include "TmxTileset.h"

#include <cassert> //RJCB

#include "TmxImage.h"
#include "TmxMap.h"
#include "TmxTerrainArray.h"

namespace Tmx
{
    namespace
    {
        auto CreateImage(const tinyxml2::XMLElement *data)
        {
            return data ? std::make_unique<Image>(data) : nullptr;
        }

        auto GetFolderName(const std::string &filename)
        {
            auto it = filename.find_last_of("/\\");
            if (it != std::string::npos)
            {
                it += 1;
            }
            return filename.substr(0, it);
        }
    }

    namespace TilesetDetails
    {
        TilesetData::TilesetData(const std::string &path, const tinyxml2::XMLElement *data)
        {
            const char *source = data->Attribute("source");
            if (!source)
            {
                filePath = path;
                this->data = data;
                return;
            }

            for (const auto &fileName : { path + source, std::string{ source } })
            {
                doc.LoadFile( fileName.c_str() );
                if (doc.ErrorID() == 0)
                {
                    filePath = GetFolderName(fileName);
                    break;
                }
            }

            if (doc.ErrorID() != 0)
            {
                fprintf(stderr, "failed to load tileset file '%s'\n", source);
                return;
            }

            // Update node and element references to the new node
            this->data = doc.FirstChildElement("tileset");
            assert(this->data); //RJCB
        }
    }

    Tileset::Tileset(const std::string &file_path, const tinyxml2::XMLElement *data)
        : Tileset{ { file_path, data }, data->IntAttribute("firstgid") }
    {
    }

    Tileset::Tileset(TilesetDetails::TilesetData data, int firstGid)
        : first_gid{ firstGid }
        , file_path{ std::move(data.filePath) }
        , name{ data->Attribute("name") }
        , tile_width{ data->IntAttribute("tilewidth") }
        , tile_height{ data->IntAttribute("tileheight") }
        , margin{ data->IntAttribute("margin") }
        , spacing{ data->IntAttribute("spacing") }
        , tile_count{ data->IntAttribute("tilecount") }
        , columns{ data->IntAttribute("columns") }
        , tileOffset{ data->FirstChildElement("tileoffset") }
        , image{ CreateImage(data->FirstChildElement("image")) }
        , properties{ data->FirstChildElement("properties") }
    {
        // Parse the terrain types if any.
        if (const auto terrainTypesNode = data->FirstChildElement("terraintypes"))
        {
            TerrainArray::Parse(&terrainTypes, terrainTypesNode);
        }

        // Iterate through all of the tile elements and parse each.
        for (auto e = data->FirstChildElement("tile"); e; e = e->NextSiblingElement("tile"))
        {
            tiles.emplace_back(e);
        }
    }

    const Tile *Tileset::GetTile(const int index) const
    {
        for (const auto &t : tiles)
        {
            if (t.GetId() == index)
            {
                return &t;
            }
        }

        return nullptr;
    }
}
