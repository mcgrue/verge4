#pragma once

#include <map>
#include <string>
#include <sdl.h>
#include "third_party/JSON/json.hpp"
#include "tileset.h"
#include "tilelayer.h"

using namespace std;

class TileMap
{
public:
	TileMap() = default;
	TileMap(const string& inputFile);
	~TileMap() = default;

	std::map<string, TileSet*> tilesets;
	std::vector<TileLayer*> indexedLayers;

	void draw(SDL_Renderer* renderer, SDL_Rect draw_area, SDL_Rect targetRect);
	void cache(SDL_Renderer* renderer, SDL_Rect draw_area,  SDL_Rect targetRect);

private:
	string pathToJSON;
	nlohmann::basic_json<> data;
	nlohmann::basic_json<> renderOrder;
	std::list<TileLayer*> orderedLayers;

	SDL_Rect cachedLocation;
	SDL_Texture* cachedBuffer = nullptr;
};
