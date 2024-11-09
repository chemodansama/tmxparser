//-----------------------------------------------------------------------------
// TmxText.cpp
//
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
//
// Author: Adaleigh Martin
//-----------------------------------------------------------------------------

#include "TmxText.h"

#include "TmxUtil.h"

namespace Tmx
{
    namespace
    {
        auto ParseHorizontalAlignment(const std::string_view s)
        {
            return
                s == "left"   ? LEFT :
                s == "center" ? HCENTER :
                s == "right"  ? RIGHT
                              : LEFT;
        }

        auto ParseVerticalAlignment(const std::string_view s)
        {
            return
                s == "top"    ? TOP :
                s == "center" ? VCENTER :
                s == "bottom" ? BOTTOM
                              : TOP;
        }
    }

    Text::Text(const tinyxml2::XMLElement *data)
        : contents{ data->GetText() ? std::string{ data->GetText() } : std::string{} }
        , font_family{ Util::ParseOrDefault(data, "fontfamily",
            [](auto attribute) { return std::string(attribute); }, "sans-serif") }
        , pixel_size{ data->IntAttribute("pixelsize", 16) }
        , color{ Util::ParseOrDefault(data, "color", [](auto attribute) { return Color(attribute); }) }
        , wrap{ data->BoolAttribute("wrap", false) }
        , bold{ data->BoolAttribute("bold", false) }
        , italic{ data->BoolAttribute("italic", false) }
        , underline{ data->BoolAttribute("underline", false) }
        , strikeout{ data->BoolAttribute("strikeout", false) }
        , kerning{ data->BoolAttribute("kerning", true) }
        , horizontal_alignment{ Util::ParseOrDefault(data, "halign", &ParseHorizontalAlignment) }
        , vertical_alignment{ Util::ParseOrDefault(data, "valign", &ParseVerticalAlignment) }
    {
    }
}
