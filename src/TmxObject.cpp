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

#include <tinyxml2.h>

#include "TmxEllipse.h"
#include "TmxMap.h"
#include "TmxPolygon.h"
#include "TmxPolyline.h"
#include "TmxText.h"

namespace Tmx
{
    namespace
    {
        template <typename T, typename... Args>
        auto ParsePrimitive(const tinyxml2::XMLNode *node, const T* patternPrimitive,
            T** primitive, Args&&... args)
        {
            if (node || patternPrimitive)
            {
                delete *primitive;

                if (node)
                {
                    *primitive = new T(std::forward<Args>(args)...);
                    (*primitive)->Parse(node);
                }
                else
                {
                    *primitive = new T(*patternPrimitive);
                }
            }
        }
    }

    Object::Object()
        : name()
        , type()
        , x(0)
        , y(0)
        , width(0)
        , height(0)
        , gid(0)
        , id(0)
        , rotation(0.0)
        , visible(true)
        , ellipse(0)
        , polygon(0)
        , polyline(0)
        , text(0)
        , properties()
    {
    }

    Object::~Object()
    {
        delete ellipse;
        delete polygon;
        delete polyline;
        delete text;
    }

    void Object::Parse(const tinyxml2::XMLNode *objectNode, Map *map)
    {
        const tinyxml2::XMLElement *objectElem = objectNode->ToElement();

        static const Object defaultPattern;
        const Object *pattern = &defaultPattern;

        const char *templateName{ nullptr };
        objectElem->QueryStringAttribute("template", &templateName);
        if (templateName)
        {
            auto templates = map->GetTemplates();
            auto it = templates.find(templateName);
            if (it == templates.end())
            {
                std::string fileName = map->GetFilepath() + templateName;
                tinyxml2::XMLDocument doc;
                doc.LoadFile(fileName.c_str());

                auto o = new Object();
                o->Parse(doc.FirstChildElement("template")->FirstChildElement("object"), map);
                pattern = templates.emplace(templateName, o).first->second;
            }
            else
            {
                pattern = it->second;
            }
        }

        // Read the attributes of the object.
        const char *tempName = objectElem->Attribute("name");
        const char *tempType = objectElem->Attribute("type");

        name = tempName ? tempName : pattern->name;
        type = tempType ? tempType : pattern->type;

        id = objectElem->IntAttribute("id");
        x = objectElem->IntAttribute("x", pattern->x);
        y = objectElem->IntAttribute("y", pattern->y);
        width = objectElem->IntAttribute("width", pattern->width);
        height = objectElem->IntAttribute("height", pattern->height);
        gid = objectElem->IntAttribute("gid");
        rotation = objectElem->IntAttribute("rotation", pattern->rotation);
        if (objectElem->QueryBoolAttribute("visible", &visible) != tinyxml2::XML_SUCCESS)
        {
            visible = pattern->visible;
        }

        // Read the ellipse of the object if there are any.
        ParsePrimitive(objectNode->FirstChildElement("ellipse"), pattern->ellipse, &ellipse,
            x, y, width, height);

        // Read the Polygon and Polyline of the object if there are any.
        ParsePrimitive(objectNode->FirstChildElement("polygon"), pattern->polygon, &polygon);
        ParsePrimitive(objectNode->FirstChildElement("polyline"), pattern->polyline, &polyline);
        ParsePrimitive(objectNode->FirstChildElement("text"), pattern->text, &text);

        // Read the properties of the object.
        const tinyxml2::XMLNode *propertiesNode = objectNode->FirstChildElement("properties");
        if (propertiesNode)
        {
            properties.Parse(propertiesNode, &pattern->properties);
        }
    }
}
