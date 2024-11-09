//-----------------------------------------------------------------------------
// Copyright (c) 2018, Adaleigh Martin
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
// DISCLAIMED. IN NO EVENT SHALL ADALEIGH MARTIN BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#include "TmxGroupLayer.h"

#include "TmxImageLayer.h"
#include "TmxLayer.h"
#include "TmxObjectGroup.h"
#include "TmxTileLayer.h"

namespace Tmx
{
    namespace
    {
    }

    GroupLayer::GroupLayer(Tmx::Map *_map, const tinyxml2::XMLElement *data)
        : Layer{ _map, data->IntAttribute("x"), data->IntAttribute("y"), 0, 0,
            TMX_LAYERTYPE_GROUP_LAYER, data }
        , offsetX{ data->IntAttribute("offsetx") }
        , offsetY{ data->IntAttribute("offsety") }
    {
        // Parse the group.
        for (auto child = data->FirstChildElement(); child; child = child->NextSiblingElement())
        {
            if (strncmp(child->Value(), "group", 5) == 0)
            {
                children.push_back(std::make_unique<GroupLayer>(map, child));
            }
            else if (strncmp(child->Value(), "layer", 5) == 0)
            {
                children.push_back(std::make_unique<TileLayer>(map, child));
            }
            else if (strncmp(child->Value(), "objectgroup", 11) == 0)
            {
                children.push_back(std::make_unique<ObjectGroup>(map, child));
            }
            else if (strncmp(child->Value(), "imagelayer", 10) == 0)
            {
                children.push_back(std::make_unique<ImageLayer>(map, child));
            }
        }
    }

    Tmx::Layer* GroupLayer::GetChild(const int index) const
    {
        return children.at(index).get();
    }

    int GroupLayer::GetNumChildren() const noexcept
    {
        return children.size();
    }

    int GroupLayer::GetOffsetX() const noexcept
    {
        return offsetX;
    }

    int GroupLayer::GetOffsetY() const noexcept
    {
        return offsetY;
    }
}
