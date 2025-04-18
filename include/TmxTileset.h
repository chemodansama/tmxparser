//-----------------------------------------------------------------------------
// TmxTileset.h
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
#pragma once

#include <memory>
#include <string>
#include <vector>

#include <tinyxml2.h>

#include "TmxPropertySet.h"
#include "TmxTerrain.h"
#include "TmxTile.h"
#include "TmxTileOffset.h"

namespace Tmx
{
    class Image;

    namespace TilesetDetails
    {
        class TilesetData
        {
        public:
            TilesetData(const std::string &path, const tinyxml2::XMLElement *data);

            const tinyxml2::XMLElement *data{ nullptr };
            std::string filePath;

            const tinyxml2::XMLElement *operator->() const { return data; }

        private:
            tinyxml2::XMLDocument doc;
        };
    }

    //-------------------------------------------------------------------------
    /// A class used for storing information about each of the tilesets.
    /// A tileset is a collection of tiles, of whom each may contain properties.
    /// This class has a property set.
    //-------------------------------------------------------------------------
    class Tileset 
    {
    public:
        Tileset(const std::string &file_path, const tinyxml2::XMLElement *data);

        /// Returns the global id of the first tile.
        int GetFirstGid() const { return first_gid; }

        const std::string &GetFilePath() const { return file_path; }

        /// Returns the name of the tileset.
        const std::string &GetName() const { return name; }

        /// Get the width of a single tile.
        int GetTileWidth() const { return tile_width; } 

        /// Get the height of a single tile.
        int GetTileHeight() const { return tile_height; }

        /// Get the margin of the tileset.
        int GetMargin() const { return margin; }

        /// Get the spacing of the tileset.
        int GetSpacing() const { return spacing; }

        /// Get the number of tiles in this tileset(since 0.13)
        int GetTileCount() const { return tile_count; }

        /// Get the number of columns in the tileset(since 0.15)
        int GetColumns() const { return columns;}

        /// Get the offset of tileset
        const Tmx::TileOffset &GetTileOffset() const { return tileOffset; }

        /// Returns a variable containing information
        /// about the image of the tileset.
        const Tmx::Image* GetImage() const { return image.get(); }

        /// Returns a a single tile of the set.
        const Tmx::Tile *GetTile(int index) const;

        /// Returns the whole tile collection.
        const std::vector<Tmx::Tile> &GetTiles() const { return tiles; }
        
        /// Get a set of properties regarding the tile.
        const Tmx::PropertySet &GetProperties() const { return properties; }

    private:
        Tileset(TilesetDetails::TilesetData data, int firstGid);

        int first_gid;
        std::string file_path;

        std::string name;
        
        int tile_width{ 0 };
        int tile_height{ 0 };
        int margin{ 0 };
        int spacing{ 0 };
        int tile_count{ 0 };
        int columns{ 0 };
        
        Tmx::TileOffset tileOffset;
        std::unique_ptr<Tmx::Image> image;

        std::vector<Tmx::Terrain> terrainTypes;
        std::vector<Tmx::Tile> tiles;
        
        Tmx::PropertySet properties;
    };
}
