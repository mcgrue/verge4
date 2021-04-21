#include <iostream>

#include "tilelayer.h"
#include "tileset.h"

#include "util/util.h"

using namespace std;

TileLayer::TileLayer(const basic_json<>& json, const basic_json<>& tileDataJson, std::map<string, TileSet*> tilesets)
{
	this->name = json["name"];
	this->parallax.x = json["parallax"]["X"];
	this->parallax.y = json["parallax"]["Y"];
	this->dimensions.x = json["dimensions"]["X"];
	this->dimensions.y = json["dimensions"]["Y"];
	this->alpha = json["alpha"];
	this->tilesetKey = json["vsp"];
	this->tileSet = tilesets[this->tilesetKey];

	auto idx = 0;
	
	for (auto entry : tileDataJson)
	{
		if (entry != 0)
		{
			auto x = getXfromFlat(idx, this->dimensions.x);
			auto y = getYfromFlat(idx, this->dimensions.x);
			this->tileData[{x, y}] = entry;
			this->hasY[y] = true;
		}

		idx++;
	}
}

#define TILE_OVERFLOW 1

void TileLayer::draw(SDL_Rect draw_area, SDL_Surface* target, SDL_Rect targetRect)
{
	if (this->alpha <= 0)
	{
		return;
	}
	clock_t beginFrame = clock();
	
	// does C++ optimize these chain lookups or should I memoize locally the "this->tileSet->tilesize.width" stuff whenever the layer's tileset changes to save a bunch of loopups every frame?
	
	// first calculate how many tiles we'll be iterating over based on this layer's tilesize
	// (this engine doesnt assume all tilesets are uniform in size!)

	int tileWidth = this->tileSet->tilesize.width;
	int tileHeight = this->tileSet->tilesize.height;
	
	int tilesWide = draw_area.w / tileWidth;
	int tilesTall = draw_area.h / tileHeight;
	
	tilesWide += TILE_OVERFLOW;
	tilesWide += TILE_OVERFLOW;
	tilesWide += TILE_OVERFLOW;
	tilesWide += TILE_OVERFLOW;
	tilesWide += TILE_OVERFLOW;
	tilesTall += TILE_OVERFLOW;
	tilesTall += TILE_OVERFLOW;

	// do we have to calc if offset and increment each by 1 if our x/y isnt on a perfect tile border?

	// now calculate the render-target's pixel offset from the perfect tile grid
	int renderOffsetX = -draw_area.x % tileWidth;
	int renderOffsetY = -draw_area.y % tileHeight;
	
	// now calculate the starting tx and ty's we need to draw.
	int originTX = draw_area.x / tileWidth;
	originTX -= TILE_OVERFLOW;
	renderOffsetX -= tileWidth * TILE_OVERFLOW;
	if (renderOffsetX != 0)
	{
		originTX--;
		renderOffsetX -= tileWidth;
		tilesWide++;
	}

	int originTY = draw_area.y / tileHeight;
	originTY -= TILE_OVERFLOW;
	renderOffsetY -= tileHeight * TILE_OVERFLOW;
	if (renderOffsetY != 0)
	{
		originTY--;
		renderOffsetY -= tileHeight;
		tilesTall++;
	}

	/// now we can draw!  Remember, Y goes on the outside loop you you draw things left to right and top down!
	for(auto y=0; y<tilesTall; y++)
	{
		if (!this->hasY[originTY+y]) continue; // skip the x's if this layer has nothing in the whole y
		
		for(auto x=0; x<tilesWide; x++)
		{
			const int tileIdx = this->tileData[{originTX + x, originTY + y}];
			if(tileIdx == 0)
			{
				continue;
			}

			int drawX = x * tileWidth + renderOffsetX;
			int drawY = y * tileHeight + renderOffsetY;
			
			tileSet->drawTile(target, tileIdx, { { drawX, drawY } });
		}
	}

	clock_t endFrame = clock();
	// LOG("render of layer " << this->name <<" took " << endFrame - beginFrame << "ms" );
}