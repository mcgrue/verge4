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

	void draw(SDL_Rect draw_area, SDL_Surface* target, SDL_Rect targetRect);

private:
	string pathToJSON;
	nlohmann::basic_json<> data;
	nlohmann::basic_json<> renderOrder;
	std::list<TileLayer*> orderedLayers;
};
