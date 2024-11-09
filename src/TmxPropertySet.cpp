//-----------------------------------------------------------------------------
// TmxPropertySet.cpp
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

#include "TmxPropertySet.h"

#include <cstdlib>

namespace Tmx
{
    namespace
    {
        auto ParsePropertiesMap(const tinyxml2::XMLNode *node)
        {
            std::unordered_map<std::string, Property> properties;
            if (!node)
            {
                return properties;
            }

            constexpr auto const property = "property";
            for (auto n = node->FirstChildElement(property); n; n = n->NextSiblingElement(property))
            {
                const auto nameAttrib = n->FindAttribute("name");
                if (nameAttrib && nameAttrib->Value()[0] != 0)
                {
                    // Read the attributes of the property and add it to the map
                    properties.emplace(nameAttrib->Value(), Property{ n });
                }
            }

            return properties;
        }

        auto AppendPatternProperties(std::unordered_map<std::string, Property> *properties,
            const PropertySet *pattern)
        {
            if (!pattern || !properties)
            {
                return;
            }

            for (const auto &p : pattern->GetPropertyMap())
            {
                if (!properties->contains(p.first))
                {
                    properties->emplace(p.first, p.second);
                }
            }
        }

        auto ParsePropertiesMap(const tinyxml2::XMLNode *node, const PropertySet *pattern)
        {
            auto properties = ParsePropertiesMap(node);
            AppendPatternProperties(&properties, pattern);
            return properties;
        }
    }

    PropertySet::PropertySet(const tinyxml2::XMLNode *propertiesNode, const PropertySet *pattern)
        : properties{ ParsePropertiesMap(propertiesNode, pattern) }
    {
    }

    const std::string &PropertySet::GetStringProperty(const std::string &name,
        const std::string &defaultValue) const
    {
        const auto it = properties.find(name);

        if (it == properties.end())
        {
            return defaultValue;
        }

        return it->second.GetValue();
    }

    int PropertySet::GetIntProperty(const std::string &name, int defaultValue) const
    {
        const auto it = properties.find(name);

        if (it == properties.end() || it->second.IsValueEmpty())
        {
            return defaultValue;
        }

        return it->second.GetIntValue();
    }

    float PropertySet::GetFloatProperty(const std::string &name, float defaultValue) const
    {
        const auto it = properties.find(name);

        if (it == properties.end() || it->second.IsValueEmpty())
        {
            return defaultValue;
        }

        return it->second.GetFloatValue();
    }

    bool PropertySet::GetBoolProperty(const std::string &name, bool defaultValue) const
    {
        const auto it = properties.find(name);

        if (it == properties.end() || it->second.IsValueEmpty())
        {
            return defaultValue;
        }

        return it->second.GetBoolValue();
    }

    Tmx::Color PropertySet::GetColorProperty(const std::string &name,
        const Tmx::Color &defaultValue) const
    {
        const auto it = properties.find(name);

        if (it == properties.end() || it->second.IsValueEmpty())
        {
            return defaultValue;
        }

        return it->second.GetColorValue(defaultValue);
    }

    bool PropertySet::HasProperty(const std::string &name) const
    {
        return properties.contains(name);
    }

    const std::unordered_map<std::string, Property> &PropertySet::GetPropertyMap() const
    {
        return properties;
    }
}
