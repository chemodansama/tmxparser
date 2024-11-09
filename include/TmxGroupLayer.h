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
#pragma once

#include <memory>
#include <string>
#include <vector>

#include <tinyxml2.h>

#include "TmxLayer.h"
#include "TmxPropertySet.h"

namespace Tmx
{
    class Map;

    //-------------------------------------------------------------------------
    /// A class used for holding groups of layers to create a layer hierarchy.
    /// This class has a property set.
    //-------------------------------------------------------------------------
    class GroupLayer : public Tmx::Layer
    {
    public:
        /// Construct an GroupLayer on the given map.
        GroupLayer(Tmx::Map *_map, const tinyxml2::XMLElement *data);

        GroupLayer(GroupLayer &&) = default;
        GroupLayer& operator=(GroupLayer &&) = default;

        Tmx::Layer* GetChild(int index) const;

        template <typename T>
        void IterateChildren(T&& fun) const;

        int GetNumChildren() const noexcept;

        /// Returns the x offset.
        int GetOffsetX() const noexcept;

        /// Returns the y offset.
        int GetOffsetY() const noexcept;

    private:
        std::vector<std::unique_ptr<Tmx::Layer>> children;

        int offsetX;
        int offsetY;
    };

    template <typename T>
    void GroupLayer::IterateChildren(T &&fun) const
    {
        for (const auto &c : children)
        {
            fun(c.get());
        }
    }
}
