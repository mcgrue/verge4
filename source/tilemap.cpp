using namespace std;

#include <iostream>
#include "tilemap.h"
#include "util/util.h"


TileMap::TileMap(const string& inputFile)
{
	const auto defJson = get_json_from_file(inputFile);
	const auto dataFile = inputFile.substr(0, inputFile.length() - 5) + ".data.json";
	const auto bulkDataJson = get_json_from_file(dataFile);

	const json layersArray = defJson["layers"];
	const json layersBulkArray = bulkDataJson["tile_data"];

	/// load the tilesets first, so we can let the layers choose the reference it needs during instance without keeping a ptr to the parent map.
	const std::map<string, string> vsps = defJson["vsp"];
	for (const auto&[key, value] : vsps) {
		std::cout << key << " = " << value << "; ";

		auto tileset = new TileSet(get_path_relative(inputFile, value));

		this->tilesets[key] = tileset;
	}
	
	if( layersArray.size() != layersBulkArray.size() )
	{
		cout << "layersCount (" << layersArray.size() << ") did not match (" << layersBulkArray.size() << ")";
		exit(2);
	}
	else
	{
		for( int i=0; i< layersArray.size(); i++ )
		{
			auto tileLayer = new TileLayer(layersArray[i], layersBulkArray[i], this->tilesets);
			indexedLayers.push_back(tileLayer);
		}
	}
	
	this->data = defJson;
	this->pathToJSON = inputFile;
		
	this->renderOrder = defJson["renderstring"];

	for ( auto item : this->renderOrder )
	{
		if( item.is_number() )
		{
			auto layerPtr = indexedLayers[item];
			orderedLayers.push_back(layerPtr);
		}
		else
		{
			cout << "skipping setting layer " << item << " for now..." << endl;
		}
	}

	/*std::string blah = source_image;	
	const auto pathToTilesetImage = get_path_relative(inputFile, blah);*/
}

void TileMap::draw( SDL_Rect draw_area, SDL_Surface* target, SDL_Rect targetRect )
{
	for( auto curLayer: this->orderedLayers )
	{
		curLayer->draw(draw_area, target, targetRect);
	}	
}

