//-----------------------------------------------------------------------------
// TmxObjectGroup.cpp
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

#include "TmxObjectGroup.h"

namespace Tmx
{
    namespace
    {
        auto ParseColor(const tinyxml2::XMLElement *data)
        {
            const auto colorAttribute = data->Attribute("color");
            return colorAttribute ? Tmx::Color(colorAttribute) : Tmx::Color{};
        }

        auto ParseObjects(const tinyxml2::XMLElement *data, Tmx::Map *map)
        {
            std::vector<Tmx::Object> objects;

            constexpr auto const object = "object";

            // Iterate through all of the object elements.
            for (auto o = data->FirstChildElement(object); o; o = o->NextSiblingElement(object))
            {
                // Add the object to the list.
                objects.emplace_back(o, map);
            }

            return objects;
        }
    }

    ObjectGroup::ObjectGroup(Tmx::Map *_map, const tinyxml2::XMLElement *data)
        : Layer{ _map, 0, 0, 0, 0, TMX_LAYERTYPE_OBJECTGROUP, data }
        , color{ ParseColor(data) }
        , objects{ ParseObjects(data, map) }
    {
    }

    ObjectGroup::ObjectGroup(const Tmx::Tile *_tile, const tinyxml2::XMLElement *data)
        : Layer{ _tile, 0, 0, 0, 0, TMX_LAYERTYPE_OBJECTGROUP, data }
        , color{ ParseColor(data) }
        , objects{ ParseObjects(data, map) }
    {
    }
}
