//-----------------------------------------------------------------------------
// TmxUtil.h
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
#include <string_view>

#include <tinyxml2.h>

namespace Tmx 
{
    namespace Util
    {
        template <typename T>
        void Iterate(const std::string_view &s, const char separator, T &&callback);

        int CountChildren(const tinyxml2::XMLElement *parent, const char *name);

        template <typename T>
        void IterateChildren(const tinyxml2::XMLElement *parent, const char *name, T &&callback);

        template <typename T>
        auto ParseOrDefault(const tinyxml2::XMLElement *data, const char *attributeName,
            T &&parser, decltype(T{}(nullptr)) defaultValue = {});

        /// Trim both leading and trailing whitespace from a string.
        std::string &Trim(std::string &str);

        /// Decode a base-64 encoded string.
        std::string DecodeBase64(const std::string &str);

        /// Decompress a gzip encoded byte array.
        char* DecompressGZIP(const char *data, int dataSize, int expectedSize);

        template <typename T>
        auto ParseOrDefault(const tinyxml2::XMLElement *data, const char *attributeName,
            T &&parser, decltype(T{}(nullptr)) defaultValue)
        {
            const auto s = data->FindAttribute(attributeName)
                ? data->Attribute(attributeName)
                : nullptr;

            return s ? parser(s) : defaultValue;
        }

        // Template implementation
        template <typename T>
        void Iterate(const std::string_view &s, const char separator, T &&callback)
        {
            if (s.empty()) {
                return;
            }

            size_t left = 0;
            for (auto it = s.find(separator); it != std::string::npos; it = s.find(separator, left))
            {
                callback(s.data() + left, s.data() + it);
                left = it + 1;
            }

            callback(s.data() + left, s.data() + s.size());
        }

        template <typename T>
        void IterateChildren(const tinyxml2::XMLElement *parent, const char *name, T &&callback)
        {
            for (auto e = parent->FirstChildElement(name); e; e = e->NextSiblingElement(name))
            {
                callback(e);
            }
        }
    }
}
