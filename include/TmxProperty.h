//-----------------------------------------------------------------------------
// TmxProperty.h
//
// Copyright (c) 2016, Tamir Atias
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
#include <variant>

#include <tinyxml2.h>

#include "TmxColor.h"

namespace Tmx
{
    //-------------------------------------------------------------------------
    /// The type of a property.
    //-------------------------------------------------------------------------
    enum PropertyType
    {
        /// A string property (default)
        TMX_PROPERTY_STRING,

        /// A boolean property
        TMX_PROPERTY_BOOL,

        /// An integer property
        TMX_PROPERTY_INT,

        /// A floating point property
        TMX_PROPERTY_FLOAT,

        /// A color property
        TMX_PROPERTY_COLOR,

        /// A file property
        TMX_PROPERTY_FILE,

        /// An object property
        TMX_PROPERTY_OBJECT,

        /// A class property
        TMX_PROPERTY_CLASS,
    };

    //-------------------------------------------------------------------------
    /// Used to store a (typed) property.
    //-------------------------------------------------------------------------
    class Property
    {
    public:
        Property(const tinyxml2::XMLElement *data);

        /// Get the type of the property (default: TMX_PROPERTY_STRING)
        PropertyType GetType() const { return type; }

        /// Check if the property is of a certain type.
        bool IsOfType(PropertyType propertyType) const { return GetType() == propertyType; }

        /// Return the value of the property.
        const std::string &GetValue() const { return std::get<std::string>(value); }

        /// Return whether the value is empty or was not specified.
        bool IsValueEmpty() const { return isEmpty; }

        /// Convert the value to a boolean and return it (or the default value if not a boolean.)
        bool GetBoolValue(bool defaultValue = false) const;

        /// Convert the value to an integer and return it (or the default value if not an integer).
        int GetIntValue(int defaultValue = 0) const;

        /// Convert the value to a float and return it (or the default value if not a float).
        float GetFloatValue(float defaultValue = 0.0f) const;

        /// Convert the value to a color and return it (or the default value if not a color).
        Tmx::Color GetColorValue(const Tmx::Color &defaultValue = Tmx::Color()) const;

    private:
        PropertyType type;
        bool isEmpty{ true };
        std::variant<std::string, int, float, bool, Tmx::Color> value;
    };

    inline bool Property::GetBoolValue(bool defaultValue) const
    {
        if (!IsOfType(TMX_PROPERTY_BOOL))
            return defaultValue;

        return std::get<bool>(value);
    }

    inline int Property::GetIntValue(int defaultValue) const
    {
        if (!IsOfType(TMX_PROPERTY_INT) && !IsOfType(TMX_PROPERTY_OBJECT))
            return defaultValue;

        return std::get<int>(value);
    }

    inline float Property::GetFloatValue(float defaultValue) const
    {
        if (!IsOfType(TMX_PROPERTY_FLOAT))
            return defaultValue;

        return std::get<float>(value);
    }

    inline Tmx::Color Property::GetColorValue(const Tmx::Color &defaultValue) const
    {
        if (!IsOfType(TMX_PROPERTY_COLOR))
            return defaultValue;

        return std::get<Tmx::Color>(value);
    }
}
