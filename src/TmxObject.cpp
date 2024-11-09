//-----------------------------------------------------------------------------
// TmxObject.cpp
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

#include "TmxObject.h"

#include "TmxEllipse.h"
#include "TmxMap.h"
#include "TmxPolygon.h"
#include "TmxPolyline.h"
#include "TmxText.h"

namespace Tmx
{
    namespace
    {
        const auto *GetDefaultPattern()
        {
            static Object defaultPattern;
            return &defaultPattern;
        }

        auto GetAttribute(const tinyxml2::XMLElement *element, const char *name,
            const std::string &defaultValue)
        {
            const auto value = element->Attribute(name);
            return value ? std::string{ value } : defaultValue;
        }

        const auto *GetOrLoadPattern(Map *map, const std::string &templateName)
        {
            if (templateName.empty())
            {
                return GetDefaultPattern();
            }

            auto &templates = map->GetTemplates();
            auto it = templates.find(templateName);
            if (it == templates.end())
            {
                tinyxml2::XMLDocument doc;

                const auto fileName = map->GetFilepath() + templateName;
                doc.LoadFile(fileName.c_str());

                if (doc.Error())
                {
                    return GetDefaultPattern();
                }

                const auto &templateData =
                    doc.FirstChildElement("template")->FirstChildElement("object");
                it = templates.emplace(templateName, Object{ templateData, map }).first;
            }

            return &it->second;
        }

        auto ParseTemplateName(const tinyxml2::XMLElement *data)
        {
            const char *templateName{ nullptr };
            data->QueryStringAttribute("template", &templateName);
            return templateName ? std::string{ templateName } : std::string{};
        }

        template <typename T, typename... Args>
        std::unique_ptr<T> ParsePrimitive(const tinyxml2::XMLElement *data, const T *pattern,
            Args&&... args)
        {
            if (data)
            {
                return std::make_unique<T>(data, std::forward<Args>(args)...);
            }

            if (pattern)
            {
                return std::make_unique<T>(*pattern);
            }

            return {};
        }
    }

    Object::Object(const tinyxml2::XMLElement *data, Map *map)
        : Object{ data, GetOrLoadPattern(map, ParseTemplateName(data)) }
    {
    }

    Object::Object(const tinyxml2::XMLElement *data, const Tmx::Object *pattern)
        : name{ GetAttribute(data, "name", pattern->name) }
        , type{ GetAttribute(data, "type", pattern->type) }
        , x{ data->IntAttribute("x", pattern->x) }
        , y{ data->IntAttribute("y", pattern->y) }
        , width{ data->IntAttribute("width", pattern->width) }
        , height{ data->IntAttribute("height", pattern->height) }
        , gid{ data->IntAttribute("gid") }
        , id{ data->IntAttribute("id") }
        , rotation{ data->FloatAttribute("rotation", pattern->rotation) }
        , visible{ data->BoolAttribute("visible", pattern->visible) }
        , ellipse{ ParsePrimitive(data->FirstChildElement("ellipse"), pattern->ellipse.get(), x, y,
            width, height) }
        , polygon{ ParsePrimitive(data->FirstChildElement("polygon"), pattern->polygon.get()) }
        , polyline{ ParsePrimitive(data->FirstChildElement("polyline"), pattern->polyline.get()) }
        , text{ ParsePrimitive(data->FirstChildElement("text"), pattern->text.get()) }
    {
    }
}
