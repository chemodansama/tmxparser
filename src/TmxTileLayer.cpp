//-----------------------------------------------------------------------------
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
//-----------------------------------------------------------------------------

#include "TmxTileLayer.h"

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstdlib>

#ifdef USE_MINIZ
#define MINIZ_HEADER_FILE_ONLY
#include "miniz.c"
#else
#include <zlib.h>
#endif

#include "TmxLayer.h"
#include "TmxUtil.h"
#include "TmxMap.h"
#include "TmxTileset.h"

namespace Tmx 
{
    TileLayer::TileLayer(Map *_map, const tinyxml2::XMLElement *data)
        : Layer{ _map, data->IntAttribute("x"), data->IntAttribute("y"),
            _map->GetWidth(), _map->GetHeight(), TMX_LAYERTYPE_TILE, data }
        , encoding(TMX_ENCODING_XML)
        , compression(TMX_COMPRESSION_NONE)
        , offsetX{ data->FloatAttribute("offsetx") }
        , offsetY{ data->FloatAttribute("offsety") }
    {
        // Allocate memory for reading the tiles.
        tile_map.reserve(width * height);

        const tinyxml2::XMLElement *dataElem = data->FirstChildElement("data");

        // Check for encoding.
        if (const char *encodingStr = dataElem->Attribute("encoding"))
        {
            encoding =
                strcmp(encodingStr, "base64") == 0 ? TMX_ENCODING_BASE64 :
                strcmp(encodingStr, "csv") == 0    ? TMX_ENCODING_CSV
                                                   : encoding;
        }

        // Check for compression.
        if (const char *compressionStr = dataElem->Attribute("compression"))
        {
            compression =
                strcmp(compressionStr, "gzip") == 0 ? TMX_COMPRESSION_GZIP :
                strcmp(compressionStr, "zlib") == 0 ? TMX_COMPRESSION_ZLIB
                                                    : compression;
        }
        
        // Decode.
        switch (encoding)
        {
        case TMX_ENCODING_XML:
            ParseXML(dataElem);
            break;

        case TMX_ENCODING_BASE64:
            ParseBase64(dataElem->GetText());
            break;

        case TMX_ENCODING_CSV:
            ParseCSV(dataElem->GetText());
            break;
        }
    }

    void TileLayer::ParseXML(const tinyxml2::XMLNode *data) 
    {
        for (auto tile = data->FirstChildElement("tile"); tile;
            tile = tile->NextSiblingElement("tile"))
        {
            // Convert to an unsigned.
            const auto gid = std::strtoul(tile->Attribute("gid"), nullptr, 10);

            // Find the tileset index.
            const int tilesetIndex = map->FindTilesetIndex(gid);
            if (tilesetIndex != -1)
            {
                // If valid, set up the map tile with the tileset.
                const Tmx::Tileset* tileset = map->GetTileset(tilesetIndex);
                tile_map.emplace_back(gid, tileset->GetFirstGid(), tilesetIndex);
            }
            else
            {
                // Otherwise, make it null.
                tile_map.emplace_back(gid, 0, -1);
            }
        }
    }

    void TileLayer::ParseBase64(std::string innerText) 
    {
    	Util::Trim(innerText);

        const std::string &text = Util::DecodeBase64(innerText);

        // Temporary array of gids to be converted to map tiles.
        unsigned *out = 0;

        if (compression == TMX_COMPRESSION_ZLIB) 
        {
            // Use zlib to uncompress the tile layer into the temporary array of tiles.
            uLongf outlen = width * height * 4;
            out = (unsigned *)malloc(outlen);
            uncompress(
                (Bytef*)out, &outlen, 
                (const Bytef*)text.c_str(), text.size());
    
        } 
        else if (compression == TMX_COMPRESSION_GZIP) 
        {
            // Use the utility class for decompressing (which uses zlib)
            out = (unsigned *)Util::DecompressGZIP(
                text.c_str(), 
                text.size(), 
                width * height * 4);
        } 
        else 
        {
            out = (unsigned *)malloc(text.size());
        
            // Copy every gid into the temporary array since
            // the decoded string is an array of 32-bit integers.
            memcpy(out, text.c_str(), text.size());
        }

        assert(out);

        // Convert the gids to map tiles.
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                unsigned gid = out[y * width + x];

                // Find the tileset index.
                const int tilesetIndex = map->FindTilesetIndex(gid);
                if (tilesetIndex != -1)
                {
                    // If valid, set up the map tile with the tileset.
                    const Tmx::Tileset* tileset = map->GetTileset(tilesetIndex);
                    tile_map.emplace_back(gid, tileset->GetFirstGid(), tilesetIndex);
                }
                else
                {
                    // Otherwise, make it null.
                    tile_map.emplace_back(gid, 0, -1);
                }
            }
        }

        // Free the temporary array from memory.
        free(out);
    }

    void TileLayer::ParseCSV(const std::string &innerText) 
    {
        Util::Iterate(innerText, ',', [this](auto first, auto last) {
            std::string_view s{ first, last };
            const auto gid = std::strtoul(s.data(), nullptr, 10);

            // Find the tileset index.
            const int tilesetIndex = map->FindTilesetIndex(gid);
            if (tilesetIndex != -1)
            {
                // If valid, set up the map tile with the tileset.
                const Tmx::Tileset *tileset = map->GetTileset(tilesetIndex);
                assert(tileset);
                tile_map.emplace_back(gid, tileset->GetFirstGid(), tilesetIndex);
            }
            else
            {
                // Otherwise, make it null.
                tile_map.emplace_back(gid, 0, -1);
            }
        });
    }
}
