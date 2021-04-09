#pragma once

#include <sdl.h>
#include <map>
#include <string>

#include "third_party/JSON/json.hpp"
#include "util/types.h"

using namespace std;
using namespace nlohmann;

// forward decls
class TileMap;
class TileSet;


class TileLayer
{
	friend TileMap;
	
public:
	TileLayer() = default;
	TileLayer(const basic_json<>& json, const basic_json<>& tileData, std::map<string, TileSet*> tilesets);
	~TileLayer() = default;

	void draw(SDL_Rect draw_area, SDL_Surface* target, SDL_Rect targetRect);

protected:
	string name;
	vec2f parallax;
	vec2i dimensions;
	float alpha = 1;
	string tilesetKey;
	TileSet* tileSet;
	
	std::map<std::pair<int, int>, int> tileData;
};
