//-----------------------------------------------------------------------------
// TmxLayer.cpp
//
// Copyright (c) 2010-2011, Tamir Atias
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
#include "TmxLayer.h"
#include "TmxUtil.h"
#include "tinyxml/tinyxml.h"
#include "zlib/zlib.h"

namespace Tmx 
{
	Layer::Layer() 
		: name() 
		, width(0) 
		, height(0) 
		, opacity(1.0f)
		, visible(true)
		, properties()
		, encoding(TMX_ENCODING_XML)
		, compression(TMX_COMPRESSION_NONE)
	{
		// Set the map to null to specify that it is not yet allocated.
		tile_map = NULL;
	}

	Layer::~Layer() 
	{
		// If the tile map is allocated, delete it from the memory.
		if (tile_map)
		{
			delete [] tile_map;
			tile_map = NULL;
		}
	}

	void Layer::Parse(const TiXmlNode *layerNode) 
	{
		const TiXmlElement *layerElem = layerNode->ToElement();
	
		// Read the attributes.
		name = layerElem->Attribute("name");

		layerElem->Attribute("width", &width);
		layerElem->Attribute("height", &height);

		const char *opacityStr = layerElem->Attribute("opacity");
		if (opacityStr) 
		{
			opacity = (float)atof(opacityStr);
		}

		const char *visibleStr = layerElem->Attribute("visible");
		if (visibleStr) 
		{
			visible = atoi(visibleStr) != 0; // to prevent visual c++ from complaining..
		}

		// Read the properties.
		const TiXmlNode *propertiesNode = layerNode->FirstChild("properties");
		if (propertiesNode) 
		{
			properties.Parse(propertiesNode);
		}

		// Allocate memory for reading the tiles.
		tile_map = new int[width * height];

		const TiXmlNode *dataNode = layerNode->FirstChild("data");
		const TiXmlElement *dataElem = dataNode->ToElement();

		const char *encodingStr = dataElem->Attribute("encoding");
		const char *compressionStr = dataElem->Attribute("compression");

		// Check for encoding.
		if (encodingStr) 
		{
			if (!strcmp(encodingStr, "base64")) 
			{
				encoding = TMX_ENCODING_BASE64;
			} else if (!strcmp(encodingStr, "csv")) 
			{
				encoding = TMX_ENCODING_CSV;
			}
		}

		// Check for compression.
		if (compressionStr) 
		{
			if (!strcmp(compressionStr, "gzip")) 
			{
				compression = TMX_COMPRESSION_GZIP;
			} 
			else if (!strcmp(compressionStr, "zlib")) 
			{
				compression = TMX_COMPRESSION_ZLIB;
			}
		}
		
		// Decode.
		switch (encoding) 
		{
		case TMX_ENCODING_XML:
			ParseXML(dataNode);
			break;

		case TMX_ENCODING_BASE64:
			ParseBase64(dataElem->GetText());
			break;

		case TMX_ENCODING_CSV:
			ParseCSV(dataElem->GetText());
			break;
		}
	}

	void Layer::ParseXML(const TiXmlNode *dataNode) 
	{
		const TiXmlNode *tileNode = dataNode->FirstChild("tile");
		int tileCount = 0;

		while (tileNode) 
		{
			const TiXmlElement *tileElem = tileNode->ToElement();
			
			// Read the Global-ID of the tile directly into the array entry.
			tileElem->Attribute("gid", &tile_map[tileCount]);
			++tileCount;

			tileNode = dataNode->IterateChildren("tile", tileNode);
		}
	}

	void Layer::ParseBase64(const std::string &innerText) 
	{
		const std::string &text = Util::DecodeBase64(innerText);

		if (compression == TMX_COMPRESSION_ZLIB) 
		{
			// Use zlib to uncompress into the map.
			uLongf outlen = width * height * 4;
			uncompress(
				(Bytef*)tile_map, &outlen, 
				(const Bytef*)text.c_str(), text.size());
	
		} 
		else if (compression == TMX_COMPRESSION_GZIP) 
		{
			// Use the utility class for decompressing (which uses zlib)
			char *out = Util::DecompressGZIP(
				text.c_str(), 
				text.size(), 
				width * height * 4);

			// Copy the decompressed buffer into the tile map
			memcpy(tile_map, out, width * height * 4);

			// Free the decompressed buffer memory since it was allocated
			// by the Util::DecompressGZIP function.
			free(out);

		} 
		else 
		{
			// Copy every into the tile_map variable since
			// the decoded string is an array of 32-bit integers.
			memcpy(tile_map, text.c_str(), text.size());
		}
	}

	void Layer::ParseCSV(const std::string &innerText) 
	{
		// Duplicate the string for use with C stdio.
		char *csv = strdup(innerText.c_str());
		
		// Iterate through every token of ';' in the CSV string.
		char *pch = strtok(csv, ";");
		int tileCount = 0;
		
		while (pch) 
		{
			tile_map[tileCount] = atoi(pch);

			++tileCount;
			pch = strtok(NULL, ";");
		}

		free(csv);
	}
};