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

	int idx = 0;
	int x, y;
	for (auto entry : tileDataJson)
	{
		if (entry != 0)
		{
			x = getXfromFlat(idx, this->dimensions.x);
			y = getYfromFlat(idx, this->dimensions.x);
			this->tileData[{x, y}] = entry;
			this->hasY[y] = true;
		}

		idx++;
	}
}


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

	int tilesWide = draw_area.w / this->tileSet->tilesize.width;
	int tilesTall = draw_area.h / this->tileSet->tilesize.height;

	// do we have to calc if offset and increment each by 1 if our x/y isnt on a perfect tile border?

	// now calculate the render-target's pixel offset from the perfect tile grid
	int renderOffsetX = -draw_area.x % this->tileSet->tilesize.width;
	int renderOffsetY = -draw_area.y % this->tileSet->tilesize.height;
	
	// now calculate the starting tx and ty's we need to draw.
	int originTX = draw_area.x / this->tileSet->tilesize.width;
	if (renderOffsetX != 0)
	{
		originTX--;
	}

	int originTY = draw_area.y / this->tileSet->tilesize.height;
	if (renderOffsetY != 0)
	{
		originTY--;
	}

	/// now we can draw!  Remember, Y goes on the outside loop you you draw things left to right and top down!
	for(auto y=0; y<tilesTall; y++)
	{
		if (!this->hasY[y]) continue; // skip the x's if this layer has nothing in the whole y
		
		for(auto x=0; x<tilesWide; x++)
		{
			const int tileIdx = this->tileData[{originTX + x, originTY + y}];
			if(tileIdx == 0)
			{
				continue;
			}

			int drawX = x * this->tileSet->tilesize.width + renderOffsetX;
			int drawY = y * this->tileSet->tilesize.height + renderOffsetY;
			
			tileSet->drawTile(target, tileIdx, { { drawX, drawY } });
		}
	}

	clock_t endFrame = clock();
	LOG("render of layer " << this->name <<" took " << endFrame - beginFrame << "ms" );
}