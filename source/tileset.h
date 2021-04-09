#pragma once

#include <sdl.h>
#include <string>
#include "third_party/JSON/json.hpp"
#include "util/types.h"

using namespace std;
using namespace nlohmann;

/// fwd decl
class TileMap;
class TileLayer;

class TileSet {

	friend TileLayer;
	//friend TileMap;
	
public:
	TileSet();
	TileSet(const string& inputFile); // simple constructor

	TileSet(const TileSet& obj) noexcept; // copy
	TileSet& operator=(TileSet& o) noexcept; //copy
	
	TileSet& operator=(TileSet&& other) noexcept; // move 
	TileSet(TileSet&& o) noexcept; // move

	~TileSet() = default; // destructor

	void drawTile(SDL_Surface* surface, tileindex_t idx, pixelcoordinates_t drawTo);
	
	SDL_Surface* getSourceImage() const;

private:
	string srcJSONPath;
	
	SDL_Surface* srcImg = nullptr;
	string absPathToSrcImg;
	basic_json<> data = nullptr;
	string pathToJSON;
	TileSize tilesize;
	int tilesPerRow;
	SDL_Rect src, dest;
};