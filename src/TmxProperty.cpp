//-----------------------------------------------------------------------------
// TmxProperty.cpp
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

#include "TmxProperty.h"

namespace Tmx
{
    namespace
    {
        PropertyType ParsePropertyType(const tinyxml2::XMLElement *data)
        {
            const auto typeAttribute = data->FindAttribute("type");
            if (!typeAttribute)
            {
                return TMX_PROPERTY_STRING;
            }

            const auto typeAsCString = typeAttribute->Value();
            if (!typeAsCString)
            {
                return TMX_PROPERTY_STRING;
            }

            return
                strcmp(typeAsCString, "string") == 0 ? TMX_PROPERTY_STRING :
                strcmp(typeAsCString, "bool") == 0   ? TMX_PROPERTY_BOOL :
                strcmp(typeAsCString, "float") == 0  ? TMX_PROPERTY_FLOAT :
                strcmp(typeAsCString, "int") == 0    ? TMX_PROPERTY_INT :
                strcmp(typeAsCString, "color") == 0  ? TMX_PROPERTY_COLOR :
                strcmp(typeAsCString, "file") == 0   ? TMX_PROPERTY_FILE :
                strcmp(typeAsCString, "object") == 0 ? TMX_PROPERTY_OBJECT :
                strcmp(typeAsCString, "class") == 0  ? TMX_PROPERTY_CLASS
                                                     : TMX_PROPERTY_STRING;
        }

        std::string ParsePropertyValue(const tinyxml2::XMLElement *data)
        {
            if (const char *valueAsCString = data->Attribute("value"))
            {
                return valueAsCString;
            }

            // The value of properties that contain newlines is stored in the element text.
            const auto text = data->GetText();
            return text ? text : std::string();
        }
    }

    Property::Property(const tinyxml2::XMLElement *data)
        : type{ ParsePropertyType(data) }
    {
        const auto valueString = ParsePropertyValue(data);
        isEmpty = valueString.empty();

        switch (type)
        {
            case TMX_PROPERTY_STRING:
            {
                value = std::move(valueString);
                break;
            }

            case TMX_PROPERTY_BOOL:
            {
                value = valueString.compare("true") == 0;
                break;
            }

            case TMX_PROPERTY_INT:
            {
                value = std::stoi(valueString);
                break;
            }

            case TMX_PROPERTY_FLOAT:
            {
                value = std::stof(valueString);
                break;
            }

            case TMX_PROPERTY_COLOR:
            {
                value = Tmx::Color(valueString);
                break;
            }

            case TMX_PROPERTY_FILE:
            {
                value = std::move(valueString);
                break;
            }

            case TMX_PROPERTY_OBJECT:
            {
                value = std::stoi(valueString);
                break;
            }

            case TMX_PROPERTY_CLASS:
            {
                //TODO: implement
                break;
            }

            default:
            {
                value = std::move(valueString);
                break;
            }
        }
    }
}
