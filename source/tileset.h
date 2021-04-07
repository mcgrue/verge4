#pragma once

#include <filesystem>
#include <sdl.h>
#include <string>
#include "third_party/JSON/json.hpp"
#include "util/util.h"

using namespace std;
using namespace nlohmann;

struct TileSize
{
public:
	int width;
	int height;
};

typedef unsigned int tileindex_t;

union vec2i {
	struct {
		int x, y;
	};
	struct {
		int w, h;
	};
	int m[2];
};

typedef vec2i pixelcoordinates_t;

class TileSet {

public:
	TileSet();
	TileSet(const string& inputFile); // simple constructor
	TileSet(const TileSet& obj); // copy constructor
	TileSet& operator=(TileSet other) noexcept;
	TileSet(TileSet&& o) noexcept; // move constructor
	~TileSet(); // destructor

	void drawTile(SDL_Surface* surface, tileindex_t idx, pixelcoordinates_t drawTo);
	
	SDL_Surface* getSourceImage();

private:
	string srcJSONPath;
	
	SDL_Surface* srcImg;
	string absPathToSrcImg;
	basic_json<> data;
	string pathToJSON;
	TileSize tilesize;
	int tilesPerRow;
	SDL_Rect src, dest;
};

inline TileSet::TileSet()
{
	this->srcImg = nullptr;
}

inline TileSet::TileSet(const string& inputFile)
{
	const auto json = get_json_from_file(inputFile);
	this->data = json;
	this->pathToJSON = inputFile;
	const auto source_image = json["source_image"];
	std::string blah = source_image;
	const auto pathToTilesetImage = get_path_relative(inputFile, blah);

	assert_file_exists(pathToTilesetImage);
	this->absPathToSrcImg = pathToTilesetImage;
	this->srcImg = load_surface(this->absPathToSrcImg);

	this->src.h = this->dest.h = this->tilesize.height = json["tilesize"]["height"];
	this->src.w = this->dest.w = this->tilesize.width = json["tilesize"]["width"];
	
	this->tilesPerRow = json["tiles_per_row"];
}

inline TileSet::TileSet(const TileSet& obj)
{
	
}

inline TileSet& TileSet::operator=(TileSet other) noexcept // call copy or move constructor to construct other
{
	std::swap(srcJSONPath, other.srcJSONPath); // exchange resources between *this and other
	std::swap(srcImg, other.srcImg);
	std::swap(data, other.data);
	std::swap(pathToJSON, other.pathToJSON);
	std::swap(tilesize, other.tilesize);
	std::swap(tilesPerRow, other.tilesPerRow);
	std::swap(dest, other.dest);
	std::swap(src, other.src);
	return *this;
}

inline TileSet::TileSet(TileSet&& o) noexcept
{
	this->srcJSONPath = std::move(o.srcJSONPath);
	this->srcImg = std::move(o.srcImg);
	this->data = std::move(o.data);
	this->pathToJSON = std::move(o.pathToJSON);
	this->tilesize = std::move(o.tilesize);
	this->tilesPerRow = std::move(o.tilesPerRow);
	this->dest = std::move(o.dest);
	this->src = std::move(o.src);
}

inline TileSet::~TileSet()
{
	
}

inline void TileSet::drawTile(SDL_Surface* surface, tileindex_t idx, pixelcoordinates_t drawTo)
{
	if(idx==0 && engine_options.never_render_tile_0)
	{
		return;
	}
	
	src.x = (idx % this->tilesPerRow) * src.w;
	src.y = (idx / this->tilesPerRow) * src.h;

	dest.x = drawTo.x;
	dest.y = drawTo.y;

	SDL_BlitSurface(srcImg, &src, surface, &dest);
}

inline SDL_Surface* TileSet::getSourceImage()
{
	return this->srcImg;
}

