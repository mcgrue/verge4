#pragma once

#include <map>
#include <string>
#include "third_party/JSON/json.hpp"
#include "tileset.h"

using namespace std;

class TileLayer
{
public:
	TileLayer() = default;
	TileLayer(const basic_json<>& json, const basic_json<>& tileData);
	~TileLayer() = default;

private:
	string name;
	vec2f parallax;
	vec2i dimensions;
	float alpha;
	string tilesetKey;
	std::map<std::pair<int, int>, int> tileData;
};

class TileMap
{
public:
	TileMap() = default;
	TileMap(const string& inputFile);
	~TileMap() = default;

	std::map<string, TileSet*> tilesets;
	std::list<TileLayer> layers;

	//void draw( Rect  );

private:
	string pathToJSON;
	nlohmann::basic_json<> data;	
};
