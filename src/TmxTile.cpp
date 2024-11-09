//-----------------------------------------------------------------------------
// TmxTile.cpp
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

#include "TmxTile.h"

#include "TmxObject.h"

namespace Tmx
{
    namespace
    {
        auto ParseType(const tinyxml2::XMLElement *data)
        {
            const auto attribute = data->Attribute("type");
            return attribute ? std::string{ attribute } : std::string{};
        }

        auto ParseFrames(const tinyxml2::XMLElement *data, bool *isAnimated,
            unsigned int *totalDuration)
        {
            std::vector<AnimationFrame> result;

            const auto animation = data->FirstChildElement("animation");
            if (!animation)
            {
                return result;
            }

            *isAnimated = true;
            *totalDuration = 0.0f;

            for (auto frame = animation->FirstChildElement("frame"); frame;
                frame = frame->NextSiblingElement("frame"))
            {
                const int tileID = frame->IntAttribute("tileid");
                const unsigned int duration = frame->IntAttribute("duration");

                result.emplace_back(tileID, duration);
                *totalDuration += duration;
            }

            return result;
        }

        std::unique_ptr<ObjectGroup> ParseObjectGroup(const Tile *tile,
            const tinyxml2::XMLElement *data)
        {
            const auto objectGroup = data->FirstChildElement("objectgroup");
            if (!objectGroup)
            {
                return nullptr;
            }

            //let's only create objectGroup if it's needed, save memory
            return std::make_unique<ObjectGroup>(tile, objectGroup);
        }

        auto ParseImage(const tinyxml2::XMLElement *data)
        {
            const auto imageNode = data->FirstChildElement("image");
            return imageNode ? std::make_unique<Image>(imageNode) : nullptr;
        }
    }

    Tile::Tile(const tinyxml2::XMLElement *data)
        : id{ data->IntAttribute("id") }
        , properties{ data->FirstChildElement("properties") }
        , type{ ParseType(data) }
        , frames{ ParseFrames(data, &isAnimated, &totalDuration) }
        , objectGroup{ ParseObjectGroup(this, data) }
        , hasObjects{ objectGroup && objectGroup->GetNumObjects() > 0 }
        , image{ ParseImage(data) }
    {
    }

    int Tile::GetFrameCount() const
    {
        return static_cast<int>(frames.size());
    }
}
