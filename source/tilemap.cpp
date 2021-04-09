using namespace std;

#include <iostream>
#include "tilemap.h"
#include "util/util.h"

TileLayer::TileLayer(const basic_json<>& json, const basic_json<>& tileDataJson)
{
	this->name = json["name"];
	this->parallax.x = json["parallax"]["X"];
	this->parallax.y = json["parallax"]["Y"];
	this->dimensions.x = json["dimensions"]["X"];
	this->dimensions.y = json["dimensions"]["Y"];
	this->alpha = json["alpha"];
	// tilesetKey =

	int idx = 0;
	int x, y;
	for (auto entry : tileDataJson)
	{
		if (entry != 0)
		{
			x = getXfromFlat(idx, this->dimensions.x);
			y = getYfromFlat(idx, this->dimensions.x);
			this->tileData[{x, y}] = entry;
		}

		idx++;
	}
}


TileMap::TileMap(const string& inputFile)
{
	const auto defJson = get_json_from_file(inputFile);
	const auto dataFile = inputFile.substr(0, inputFile.length() - 5) + ".data.json";
	const auto bulkDataJson = get_json_from_file(dataFile);

	const json layersArray = defJson["layers"];
	const json layersBulkArray = bulkDataJson["tile_data"];

	if( layersArray.size() != layersBulkArray.size() )
	{
		cout << "layersCount (" << layersArray.size() << ") did not match (" << layersBulkArray.size() << ")";
		exit(2);
	}
	else
	{
		for( int i=0; i< layersArray.size(); i++ )
		{
			auto tileLayer = TileLayer(layersArray[i], layersBulkArray[i]);
			layers.push_back(tileLayer);
		}
	}
	
	this->data = defJson;
	this->pathToJSON = inputFile;
	
	const std::map<string, string> vsps = defJson["vsp"];
	for (const auto&[key, value] : vsps) {
		std::cout << key << " = " << value << "; ";
		
		auto tileset = new TileSet(get_path_relative(inputFile, value));

		this->tilesets[key] = tileset;
	}
	
	/*std::string blah = source_image;	
	const auto pathToTilesetImage = get_path_relative(inputFile, blah);*/
}

//void TileMap::draw()
//{
//	
//}

