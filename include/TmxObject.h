//-----------------------------------------------------------------------------
// TmxObject.h
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

#include <tinyxml2.h>

#include "TmxPropertySet.h"

#include "TmxEllipse.h"
#include "TmxPolygon.h"
#include "TmxPolyline.h"
#include "TmxText.h"

namespace Tmx
{
    class Map;

    //-------------------------------------------------------------------------
    /// Class used for representing a single object from the objectgroup.
    //-------------------------------------------------------------------------
    class Object
    {
    public:
        Object();
        Object(const tinyxml2::XMLElement *data, Map *map);

        /// Get the name of the object.
        const std::string &GetName() const { return name; }

        /// Get the type of the object.
        const std::string &GetType() const { return type; }

        /// Get the left side of the object, in pixels.
        int GetX() const { return x; }

        /// Get the top side of the object, in pixels.
        int GetY() const { return y; }

        /// Get the width of the object, in pixels.
        int GetWidth() const { return width; }

        /// Get the height of the object, in pixels.
        int GetHeight() const { return height; }

        /// Get the rotation of the object, in degrees.
        float GetRot() const { return rotation; }

        /// Get the Global ID of the tile associated with this object.
        int GetGid() const { return gid; }

        /// Get the ID of this object.
        int GetId() const { return id; }

        /// Get the visibility of the object.
        bool IsVisible() const { return visible; }

        /// Get the ellipse.
        const Tmx::Ellipse *GetEllipse() const { return ellipse.get(); }

        /// Get the Polygon.
        const Tmx::Polygon *GetPolygon() const { return polygon.get(); }

        /// Get the Polyline.
        const Tmx::Polyline *GetPolyline() const { return polyline.get(); }

        /// Get the Text.
        const Tmx::Text *GetText() const { return text.get(); }

        /// Get the property set.
        const Tmx::PropertySet &GetProperties() const { return properties; }

    private:
        Object(const tinyxml2::XMLElement *data, const Tmx::Object *pattern);

        std::string name;
        std::string type;

        int x{ 0 };
        int y{ 0 };
        int width{ 0 };
        int height{ 0 };
        int gid{ 0 };
        int id{ 0 };

        float rotation{ 0.0f };
        bool visible{ true };

        std::unique_ptr<Tmx::Ellipse> ellipse;
        std::unique_ptr<Tmx::Polygon> polygon;
        std::unique_ptr<Tmx::Polyline> polyline;
        std::unique_ptr<Tmx::Text> text;

        Tmx::PropertySet properties;
    };
}
