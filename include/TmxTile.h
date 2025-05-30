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
#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <tinyxml2.h>

#include "TmxImage.h"
#include "TmxObjectGroup.h"
#include "TmxPropertySet.h"

namespace Tmx
{
    class AnimationFrame;
    class Object;

    //-------------------------------------------------------------------------
    /// Class to contain information about every tile in the tileset/tiles
    /// element.
    /// It may expand if there are more elements or attributes added into the
    /// the tile element.
    /// This class also contains a property set.
    //-------------------------------------------------------------------------
    class Tile
    {
    public:
        /// Construct a new tile with the given id.
        Tile(const tinyxml2::XMLElement *data);

        /// Get the Id. (relative to the tileset)
        int GetId() const
        {
            return id;
        }

        /// Returns true if the tile is animated (has one or more animation frames)
        bool IsAnimated() const
        {
            return isAnimated;
        }

        /// Returns the number of frames of the animation. If the tile is not animated, returns 0.
        int GetFrameCount() const;

        /// Returns the total duration of the animation, in milliseconds,
        /// or 0 if the tile is not animated.
        unsigned int GetTotalDuration() const
        {
            return totalDuration;
        }

        /// Returns the tile image if defined.
        const Tmx::Image* GetImage() const
        {
            return image.get();
        }

        /// Returns the object type of the tile.
        std::string GetType() const
        {
            return type;
        }

        /// Returns the frames of the animation.
        const std::vector<AnimationFrame> &GetFrames() const
        {
            return frames;
        }

        /// Get a set of properties regarding the tile.
        const Tmx::PropertySet &GetProperties() const
        {
            return properties;
        }

        //// Get the object group, which contains additional tile properties
        const Tmx::ObjectGroup *GetObjectGroup() const
        {
            return objectGroup.get();
        }

        //// Get the object group's properties, convenience function
        const Tmx::PropertySet &GetObjectGroupProperties() const
        {
            if (!objectGroup)
            {
                throw std::runtime_error("Tile has no ObjectGroup on attempt to get "
                    "ObjectGroup properties.  Cannot return null ref.");
            }
            return objectGroup->GetProperties();
        }

        /// Get set of Collision Objects, convenience function
        const std::vector<Tmx::Object> &GetObjects() const
        {
            if (!objectGroup)
            {
                throw std::out_of_range("Tile has no objectGroup");
            }

            return objectGroup->GetObjects();
        }

        /// Returns true if tile has Collision Objects
        bool HasObjects() const
        {
            return hasObjects;
        }

        /// Get a single object.
        const Tmx::Object &GetObject(int index) const
        {
            if (!objectGroup)
            {
                throw std::out_of_range("Tile has no objectGroup");
            }

            return objectGroup->GetObject(index);
        }

        /// Get the number of objects in the list.
        int GetNumObjects() const
        {
            if (!objectGroup)
            {
                throw std::out_of_range("Tile has no objectGroup");
            }
            return objectGroup->GetNumObjects();
        }

    private:
        int id{ 0 };

        Tmx::PropertySet properties;

        std::string type;

        bool isAnimated{ false };
        unsigned int totalDuration{ 0 };
        std::vector<AnimationFrame> frames;

        std::unique_ptr<Tmx::ObjectGroup> objectGroup;
        bool hasObjects{ false };

        std::unique_ptr<Tmx::Image> image;
    };

    //-------------------------------------------------------------------------
    /// Class containing information about an animated tile. This includes the
    /// duration of each frame and the various ids of each frame in the
    /// animation.
    //-------------------------------------------------------------------------
    class AnimationFrame
    {
    public:
        /// Create a new animation frame with a specified tile id and duration.
        AnimationFrame(int tileID = -1, unsigned int duration = 0)
            : tileID(tileID)
            , duration(duration)
        {
        }

        /// Get the tile id of this frame, relative to the containing tileset.
        int GetTileID() const
        {
            return tileID;
        }

        /// Get the duration of this frame in milliseconds.
        unsigned int GetDuration() const
        {
            return duration;
        }

    private:
        int tileID;
        unsigned int duration;
    };
}
