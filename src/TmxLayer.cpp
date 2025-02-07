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

#include "TmxLayer.h"

#include <cstdlib>

#ifdef USE_MINIZ
#define MINIZ_HEADER_FILE_ONLY
#include "miniz.c"
#else
#include <zlib.h>
#endif

#include "TmxMap.h"
#include "TmxTileset.h"
#include "TmxUtil.h"

namespace
{
    // Avoid nextParseOrder to be included in the documentation as it is an implementation detail that should not be considered as a part of the API.
    /// @cond INTERNAL
    int nextParseOrder = 0;
    /// @endcond
}

namespace Tmx
{
    namespace
    {
        auto ParseName(const tinyxml2::XMLElement * const data)
        {
            const auto nameAttribute = data->Attribute("name");
            return nameAttribute ? std::string{ nameAttribute } : std::string{};
        }

        auto GetFloatAttribute(const tinyxml2::XMLElement * const data,
            const char * const name, const float defaultValue)
        {
            const auto a = data->Attribute(name);
            return a ? std::strtof(a, nullptr) : defaultValue;
        };

        auto GetBoolAttribute(const tinyxml2::XMLElement * const data,
            const char * const name, const bool defaultValue)
        {
            bool result{ defaultValue };
            data->QueryBoolAttribute("visible", &result);
            return result;
        }
    }

    Layer::Layer(Tmx::Map *_map, const int _x, const int _y,
        const int _width, const int _height, const LayerType _layerType,
        const tinyxml2::XMLElement *data)
        : Layer{ _map, nullptr, _x, _y, _width, _height, _layerType, data }
    {
    }

    Layer::Layer(const Tmx::Tile *_tile, const int _x, const int _y,
        const int _width, const int _height, const LayerType _layerType,
        const tinyxml2::XMLElement *data)
        : Layer{ nullptr, _tile, _x, _y, _width, _height, _layerType, data }
    {
    }

    Layer::Layer(Tmx::Map *_map, const Tmx::Tile *_tile, int _x, int _y,
        int _width, int _height, LayerType _layerType, const tinyxml2::XMLElement *data)
        : map(_map)
        , tile(_tile)
        , name(ParseName(data))
        , x(_x)
        , y(_y)
        , width(_width)
        , height(_height)
        , opacity(GetFloatAttribute(data, "opacity", 1.0f))
        , visible(GetBoolAttribute(data, "visible", true))
        , zOrder(nextParseOrder)
        , parseOrder(nextParseOrder)
        , parallaxX{ GetFloatAttribute(data, "parallaxx", 1.0f) }
        , parallaxY{ GetFloatAttribute(data, "parallaxy", 1.0f) }
        , offsetX{ GetFloatAttribute(data, "offsetx", 1.0f) }
        , offsetY{ GetFloatAttribute(data, "offsety", 1.0f) }
        , layerType(_layerType)
        , properties(data->FirstChildElement("properties"))
    {
        ++nextParseOrder;
    }
}
