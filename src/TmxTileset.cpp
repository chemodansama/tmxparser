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
    }

    Tileset::Tileset(const std::string &file_path, const tinyxml2::XMLNode *tilesetNode)
        : first_gid(0)
        , name()
        , tile_width(0)
        , tile_height(0)
        , margin(0)
        , spacing(0)
        , tile_count(0)
        , columns(0)
        , tileOffset{ tilesetNode->FirstChildElement("tileoffset") }
        , image{ CreateImage(tilesetNode->FirstChildElement("image")) }
        , tiles()
        , properties{ tilesetNode->FirstChildElement("properties") }
    {
        const tinyxml2::XMLElement *tilesetElem = tilesetNode->ToElement();

        // Read all the attributes into local variables.

        // The firstgid and source attribute are kept in the TMX map,
        // since they are map specific.
        first_gid = tilesetElem->IntAttribute("firstgid");

        // If the <tileset> node contains a 'source' tag,
        // the tileset config should be loaded from an external
        // TSX (Tile Set XML) file. That file has the same structure
        // as the <tileset> element in the TMX map.
        const char* source_name = tilesetElem->Attribute("source");
        tinyxml2::XMLDocument tileset_doc;
        if ( source_name )
        {
            for (const auto &fileName : { file_path + source_name, std::string{ source_name } })
            {
                auto it = fileName.find_last_of("/\\");
                if (it != std::string::npos)
                {
                    it += 1;
                }

                this->file_path = fileName.substr(0, it);

                tileset_doc.LoadFile( fileName.c_str() );
                if (tileset_doc.ErrorID() == 0)
                {
                    break;
                }
            }

            if ( tileset_doc.ErrorID() != 0)
            {
                fprintf(stderr, "failed to load tileset file '%s'\n", source_name);
                return;
            }

            // Update node and element references to the new node
            tilesetNode = tileset_doc.FirstChildElement("tileset");
            assert(tilesetNode); //RJCB

            tilesetElem = tilesetNode->ToElement();
        }
        else
        {
            this->file_path = file_path;
        }

        tile_width = tilesetElem->IntAttribute("tilewidth");
        tile_height = tilesetElem->IntAttribute("tileheight");
        margin = tilesetElem->IntAttribute("margin");
        spacing = tilesetElem->IntAttribute("spacing");
        tile_count = tilesetElem->IntAttribute("tilecount");
        columns = tilesetElem->IntAttribute("columns");
        name = tilesetElem->Attribute("name");

        // Parse the terrain types if any.
        if (const auto terrainTypesNode = tilesetNode->FirstChildElement("terraintypes"))
        {
             TerrainArray::Parse(&terrainTypes, terrainTypesNode);
        }

        // Iterate through all of the tile elements and parse each.
        auto tileNode = tilesetNode->FirstChildElement("tile");
        for (int tId = 0; tileNode; ++tId)
        {
            tiles.emplace_back(tileNode);
            tileNode = tileNode->NextSiblingElement("tile");
        }
    }

    const Tile *Tileset::GetTile(int index) const
    {
        for (size_t i = 0; i < tiles.size(); ++i)
        {
            if (tiles.at(i).GetId() == index)
                return &tiles.at(i);
        }

        return nullptr;
    }
}
