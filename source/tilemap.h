#pragma once

#include <string>
#include "third_party/JSON/json.hpp"

using namespace std;

class TileMap {

public:
	TileMap(string inputFile); // simple constructor
	TileMap(const TileMap &obj);		// copy constructor
	~TileMap();						// destructor

private:
	string srcJSONPath;
	nlohmann::basic_json<> data;
};