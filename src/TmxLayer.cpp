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

#include <algorithm>
#include <cstdlib>
#include <cstdio>

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
    Layer::Layer(Tmx::Map *_map, const std::string _name, const int _x, const int _y, const int _width, const int _height, const float _opacity, const bool _visible, const LayerType _layerType)
        : map(_map)
        , tile(NULL)
        , name(_name)
        , x(_x)
        , y(_y)
        , width(_width)
        , height(_height)
        , opacity(_opacity)
        , visible(_visible)
        , zOrder(nextParseOrder)
        , parseOrder(nextParseOrder)
        , layerType(_layerType)
        , properties()
    {
        ++nextParseOrder;
    }

    Layer::Layer(const Tmx::Tile *_tile, const std::string _name, const int _x, const int _y, const int _width, const int _height, const float _opacity, const bool _visible, const LayerType _layerType) 
        : map(NULL)
        , tile(_tile)
        , name(_name)
        , x(_x)
        , y(_y)
        , width(_width)
        , height(_height)
        , opacity(_opacity)
        , visible(_visible)
        , zOrder(nextParseOrder)
        , parseOrder(nextParseOrder)
        , layerType(_layerType)
        , properties()
    {
        ++nextParseOrder;
    }

    void Layer::Parse(const tinyxml2::XMLNode *layerNode)
    {
        const tinyxml2::XMLElement *layerElement = layerNode->ToElement();

        const auto nameAttribute = layerElement->Attribute("name");
        name = nameAttribute ? std::string{ nameAttribute } : std::string{};

        const auto floatAttribute = [layerElement](const char *name, auto defaultValue) {
            const auto a = layerElement->Attribute(name);
            return a ? std::atof(a) : defaultValue;
        };

        parallaxX = floatAttribute("parallaxx", parallaxX);
        parallaxY = floatAttribute("parallaxy", parallaxY);
    }
}
