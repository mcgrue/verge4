#include <optional>
using namespace std;

#include <sdl.h>
#include <iostream>

#include "tilemap.h"
#include "util/util.h"


TileMap::TileMap(const string& input_file)
{
	const auto def_JSON = get_json_from_file(input_file);

	set_window_title(def_JSON["name"]);
	
	const auto data_file = input_file.substr(0, input_file.length() - 5) + ".data.json";
	const auto bulkDataJson = get_json_from_file(data_file);

	const json &layers_array = def_JSON["layers"];
	const json &layers_bulk_array = bulkDataJson["tile_data"];

	/// load the tilesets first, so we can let the layers choose the reference it needs during instance without keeping a ptr to the parent map.
	const std::map<string, string> vsps = def_JSON["vsp"];
	for (const auto&[key, value] : vsps) {
		LOG( key << " = " << value << "; " );

		TileSet* tile_set = new TileSet(get_path_relative(input_file, value));

		this->tilesets[key] = tile_set;
	}
	
	if( layers_array.size() != layers_bulk_array.size() )
	{
		LOG( "layersCount (" << layers_array.size() << ") did not match (" << layers_bulk_array.size() << ")" );
		exit(2);
	}
	else
	{		
		for( int i=0; i< layers_array.size(); i++ )
		{
			set_window_title(layers_array[i]["name"]);
			
			auto tileLayer = new TileLayer(layers_array[i], layers_bulk_array[i], this->tilesets);
			indexedLayers.push_back(tileLayer);
		}
	}

	/// This engine calculates a "Map's Size" on the dimensions of the base layer currently.
	/// TODO: Consider having this as another top-level JSON variable in the definition file
	TileLayer* baseLayer = indexedLayers[0];
	tilesize_t baseTilesize = baseLayer->tileSet->tilesize();
	int mapPW = baseTilesize.w * baseLayer->dimensions.x;
	int mapPH = baseTilesize.h * baseLayer->dimensions.y;
	this->_mapsizeInPixels = {0,0,mapPW,mapPH };
	
	this->data = def_JSON;
	this->pathToJSON = input_file;
		
	this->renderOrder = def_JSON["renderstring"];

	{
		string tmp = def_JSON["render_mode"];
		
		transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
		
		if(tmp == "wrap_x")
		{
			this->_map_wrap_mode = WRAP_X;
		}
		else if (tmp == "wrap_y")
		{
			this->_map_wrap_mode = WRAP_Y;
		}
		else if (tmp == "wrap_x")
		{
			this->_map_wrap_mode = WRAP_BOTH;
		}
		else
		{
			this->_map_wrap_mode = NO_WRAP;
		}
	}

	for ( auto const item : this->renderOrder )
	{
		if( item.is_number() )
		{
			TileLayer* layerPtr = indexedLayers[item];
			orderedLayers.push_back(layerPtr);
		}
		else
		{
			LOG("skipping setting layer " << item << " for now...");
		}
	}
}

void TileMap::draw(SDL_Renderer* renderer, SDL_Rect draw_area,  SDL_Rect targetRect )
{
	if (
		engine_options.naive_caching &&
		draw_area.x == this->cachedLocation.x &&
		draw_area.y == this->cachedLocation.y &&
		draw_area.h == this->cachedLocation.h &&
		draw_area.w == this->cachedLocation.w 
	)
	{
		SDL_RenderCopy(renderer, this->cachedBuffer, &targetRect, &targetRect);
		return;
	}
	
	clock_t beginFrame = clock();
	for( auto curLayer: this->orderedLayers )
	{
		curLayer->draw(renderer, draw_area, targetRect);
	}

	clock_t endFrame = clock();
	// LOG( "full map render took " << endFrame - beginFrame << "ms" );

	if(engine_options.naive_caching) {
		cache(renderer, draw_area, targetRect);
	}
}


void TileMap::cache(SDL_Renderer* renderer, SDL_Rect draw_area, SDL_Rect targetRect)
{
	this->cachedLocation = draw_area;

	this->cachedBuffer = get_current_screen_as_texture();

	
	// SDL_BlitSurface(renderer, &targetRect, this->cachedBuffer, &targetRect);
	//if(SDL_SaveBMP(this->cachedBuffer, "c:/tmp/frame.bmp") != 0)
	//{
	//	// Error saving bitmap
	//	printf("SDL_SaveBMP failed: %s\n", SDL_GetError());
	//}
}
