//-----------------------------------------------------------------------------
// TmxObjectGroup.h
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

#include <string>
#include <vector>

#include <tinyxml2.h>

#include "TmxLayer.h"
#include "TmxObject.h"

namespace Tmx
{
    class Map;

    //-------------------------------------------------------------------------
    /// A class used for holding a list of objects.
    /// This class has a property set.
    //-------------------------------------------------------------------------
    class ObjectGroup : public Tmx::Layer
    {
    public:
        /// Construct a new ObjectGroup
        ObjectGroup(Tmx::Map *_map, const tinyxml2::XMLElement *data);

        /// Construct a new ObjectGroup used by a Tile
        ObjectGroup(const Tmx::Tile *_tile, const tinyxml2::XMLElement *data);

        ObjectGroup(ObjectGroup &&) = default;
        ObjectGroup& operator=(ObjectGroup &&) = default;

        /// Get a single object.
        const Tmx::Object &GetObject(int index) const { return objects.at(index); }

        /// Get the number of objects in the list.
        int GetNumObjects() const { return objects.size(); }

        /// Get the color used to display the objects in this group.
        Tmx::Color GetColor() const { return color; }

        /// Get the whole list of objects.
        const std::vector<Tmx::Object> &GetObjects() const { return objects; }

    private:
        Tmx::Color color;
        std::vector<Tmx::Object> objects;
    };
}
