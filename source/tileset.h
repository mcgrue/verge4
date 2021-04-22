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

private:
	string src_JSON_path;
	SDL_Texture* src_img = nullptr;
	string abs_path_to_src_img;
	basic_json<> data = nullptr;
	string path_to_json;
	tilesize_t _tile_size;
	int tiles_per_row;
	SDL_Rect src, dest;
	
	
public:
	TileSet();
	TileSet(const string& inputFile); // simple constructor

	TileSet(const TileSet& obj) noexcept; // copy
	TileSet& operator=(TileSet& o) noexcept; //copy
	
	TileSet& operator=(TileSet&& other) noexcept; // move 
	TileSet(TileSet&& o) noexcept; // move

	~TileSet() = default; // destructor

	void drawTile(SDL_Renderer* renderer, tileindex_t idx, pixelcoordinates_t drawTo);
	
	SDL_Texture* getSourceImage() const;

	tilesize_t tilesize() const { return this->_tile_size; }
};