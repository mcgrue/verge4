#pragma once

#include <map>
#include <string>
#include <sdl.h>
#include "third_party/JSON/json.hpp"
#include "tileset.h"
#include "tilelayer.h"
#include "util/util.h"

using namespace std;

class TileMap
{
private:
	string pathToJSON;
	nlohmann::basic_json<> data;
	nlohmann::basic_json<> renderOrder;
	std::list<TileLayer*> orderedLayers;

	SDL_Rect cachedLocation;
	SDL_Texture* cachedBuffer = nullptr;

	SDL_Rect _mapsizeInPixels;
	map_wrap_mode _map_wrap_mode;
	
public:
	TileMap() = default;
	TileMap(const string& input_file);
	~TileMap() = default;

	std::map<string, TileSet*> tilesets;
	std::vector<TileLayer*> indexedLayers;

	void draw(SDL_Renderer* renderer, SDL_Rect draw_area, SDL_Rect targetRect);
	void cache(SDL_Renderer* renderer, SDL_Rect draw_area,  SDL_Rect targetRect);

	[[nodiscard]] SDL_Rect size() const { return _mapsizeInPixels; }
	[[nodiscard]] map_wrap_mode wrap_mode() const { return _map_wrap_mode; }
};
