//-----------------------------------------------------------------------------
// TmxMap.h
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
#include <unordered_map>
#include <vector>

#include "TmxPropertySet.h"

namespace tinyxml2
{
    class XMLNode;
}

namespace Tmx
{
    class Layer;
    class TileLayer;
    class ImageLayer;
    class Object;
    class ObjectGroup;
    class GroupLayer;
    class Tileset;

    //-------------------------------------------------------------------------
    /// Error in handling of the Map class.
    //-------------------------------------------------------------------------
    enum MapError
    {
        /// A file could not be opened. (usually due to permission problems)
        TMX_COULDNT_OPEN = 0x01,

        /// There was an error in parsing the TMX file.
        /// This is being caused by TinyXML parsing problems.
        TMX_PARSING_ERROR = 0x02,

        /// The size of the file is invalid.
        TMX_INVALID_FILE_SIZE = 0x04
    };

    //-------------------------------------------------------------------------
    /// The way the map is viewed.
    //-------------------------------------------------------------------------
    enum MapOrientation
    {
        /// This map is an orthogonal map.
        TMX_MO_ORTHOGONAL = 0x01,

        /// This map is an isometric map.
        TMX_MO_ISOMETRIC = 0x02,

        /// This map is an isometric staggered map.
        TMX_MO_STAGGERED = 0x03,

        /// This map is an hexagonal staggered map.
        TMX_MO_HEXAGONAL = 0x04
    };

    //-------------------------------------------------------------------------
    /// The order in which tiles on tile layers are rendered.
    /// The default is TMX_RIGHT_DOWN.
    //-------------------------------------------------------------------------
    enum MapRenderOrder
    {
        TMX_RIGHT_DOWN = 0x01,
        TMX_RIGHT_UP = 0x02,
        TMX_LEFT_DOWN = 0x03,
        TMX_LEFT_UP= 0x04
    };

    //-------------------------------------------------------------------------
    /// The stagger axis for the map. (only applies to hexagonal maps)
    //-------------------------------------------------------------------------
    enum MapStaggerAxis
    {
        TMX_SA_NONE = 0x00,  // if the map is not hexagonal
        TMX_SA_X = 0x01,
        TMX_SA_Y = 0x02
    };

    //-------------------------------------------------------------------------
    /// The stagger index for the map. (applies to hexagonal AND isometric staggered maps)
    //-------------------------------------------------------------------------
    enum MapStaggerIndex
    {
        TMX_SI_NONE = 0x00,  // if the map is not hexagonal
        TMX_SI_EVEN = 0x01,
        TMX_SI_ODD = 0x02
    };

    //-------------------------------------------------------------------------
    /// This class is the root class of the parser.
    /// It has all of the information in regard to the TMX file.
    /// This class has a property set.
    //-------------------------------------------------------------------------
    class Map
    {
    public:
        /// Read a file and parse it.
        /// Note: use '/' instead of '\\' as it is using '/' to find the path.
        static Map ParseFile(const std::string &fileName);

        /// Parse text containing TMX formatted XML.
        static Map ParseText(const std::string &text, const std::string &path = "");
        static Map ParseText(const char *text, const std::string &path = "");

        /// Get a path to the directory of the map file if any.
        const std::string &GetFilepath() const { return file_path; }

        /// Get the background color of the map file. If unset, return a fully transparent color.
        Tmx::Color GetBackgroundColor() const { return background_color; }

        /// Get the version of the map.
        double GetVersion() const { return version; }

        /// Get the orientation of the map.
        Tmx::MapOrientation GetOrientation() const { return orientation; }

        /// Get the render order of the map.
        Tmx::MapRenderOrder GetRenderOrder() const { return render_order; }

        /// Get the stagger axis of the map.
        Tmx::MapStaggerAxis GetStaggerAxis() const { return stagger_axis; }

        /// Get the stagger index of the map.
        Tmx::MapStaggerIndex GetStaggerIndex() const { return stagger_index; }

        /// Get the width of the map, in tiles.
        int GetWidth() const { return width; }

        /// Get the height of the map, in tiles.
        int GetHeight() const { return height; }

        /// Get the width of a tile, in pixels.
        int GetTileWidth() const { return tile_width; }

        /// Get the height of a tile, in pixels.
        int GetTileHeight() const { return tile_height; }

        /// Get the next object id.
        int GetNextObjectId() const { return next_object_id; }

        /// Get the hexside length.
        int GetHexsideLength() const { return hexside_length; }

        float GetParallaxOriginX() const { return parallaxOriginX; }
        float GetParallaxOriginY() const { return parallaxOriginY; }

        bool IsInfinite() const { return infinite; }

        /// Get the layer at a certain index.
        const Tmx::Layer *GetLayer(int index) const;

        /// Get the amount of layers.
        int GetNumLayers() const;

        /// Get the whole layers collection.
        const std::vector<Tmx::Layer*> &GetLayers() const { return layers; }

        /// Get the tile layer at a certain index.
        const Tmx::TileLayer *GetTileLayer(int index) const;

        /// Get the amount of tile layers.
        int GetNumTileLayers() const;

        /// Get the whole collection of tile layers.
        const std::vector<Tmx::TileLayer> &GetTileLayers() const { return tile_layers; }

        /// Get the object group at a certain index.
        const Tmx::ObjectGroup *GetObjectGroup(int index) const;

        /// Get the amount of object groups.
        int GetNumObjectGroups() const;

        /// Get the whole collection of object groups.
        const std::vector<Tmx::ObjectGroup> &GetObjectGroups() const;

        std::unordered_map<std::string, Tmx::Object> &GetTemplates() { return templates; }

        /// Get the image layer at a certain index.
        const Tmx::ImageLayer *GetImageLayer(int index) const;

        /// Get the amount of image layers.
        int GetNumImageLayers() const;

        /// Get the whole collection of image layers.
        const std::vector<Tmx::ImageLayer> &GetImageLayers() const { return image_layers; }

        const Tmx::GroupLayer *GetGroupLayer(int index) const;
        int GetNumGroupLayers() const;

        const std::vector<Tmx::GroupLayer> &GetGroupLayers() const { return group_layers; }

        /// Find the tileset index for a tileset using a tile gid.
        int FindTilesetIndex(int gid) const;

        /// Find a tileset for a specific gid.
        const Tmx::Tileset *FindTileset(int gid) const;

        /// Get a tileset by an index.
        const Tmx::Tileset *GetTileset(int index) const;

        /// Get the amount of tilesets.
        int GetNumTilesets() const;

        /// Get the collection of tilesets.
        const std::vector<Tmx::Tileset> &GetTilesets() const { return tilesets; }

        /// Get whether there was an error or not.
        bool HasError() const { return has_error; }

        /// Get an error string containing the error in text format.
        const std::string &GetErrorText() const { return error_text; }

        /// Get a number that identifies the error. (TMX_ preceded constants)
        unsigned char GetErrorCode() const { return error_code; }

        /// Get the property set.
        const Tmx::PropertySet &GetProperties() const { return properties; }

    private:
        Map(std::string errorText);
        Map(const tinyxml2::XMLElement *data, std::string filePath);

        std::string file_path;

        Tmx::Color background_color;

        double version{ 0.0 };
        Tmx::MapOrientation orientation{ Tmx::MapOrientation::TMX_MO_ORTHOGONAL };
        Tmx::MapRenderOrder render_order{ Tmx::MapRenderOrder::TMX_RIGHT_DOWN };
        Tmx::MapStaggerAxis stagger_axis{ Tmx::MapStaggerAxis::TMX_SA_NONE };
        Tmx::MapStaggerIndex stagger_index{ Tmx::MapStaggerIndex::TMX_SI_NONE };

        int width{ 0 };
        int height{ 0 };
        int tile_width{ 0 };
        int tile_height{ 0 };
        int next_object_id{ 0 };
        int hexside_length{ 0 };

        float parallaxOriginX{ 0.0f };
        float parallaxOriginY{ 0.0f };

        bool infinite{ false };

        std::vector<Tmx::Layer*> layers;
        std::vector<Tmx::TileLayer> tile_layers;
        std::vector<Tmx::ImageLayer> image_layers;
        std::vector<Tmx::ObjectGroup> object_groups;
        std::vector<Tmx::GroupLayer> group_layers;
        std::vector<Tmx::Tileset> tilesets;
        std::unordered_map<std::string, Tmx::Object> templates;

        bool has_error{ false };
        unsigned char error_code{ 0 };
        std::string error_text;

        Tmx::PropertySet properties;
    };
}
