using namespace std;

#include <sdl.h>
#include <iostream>

#include "tilemap.h"
#include "util/util.h"


TileMap::TileMap(const string& inputFile)
{
	const auto defJson = get_json_from_file(inputFile);


	set_window_title(defJson["name"]);
	
	const auto dataFile = inputFile.substr(0, inputFile.length() - 5) + ".data.json";
	const auto bulkDataJson = get_json_from_file(dataFile);

	const json &layersArray = defJson["layers"];
	const json &layersBulkArray = bulkDataJson["tile_data"];

	/// load the tilesets first, so we can let the layers choose the reference it needs during instance without keeping a ptr to the parent map.
	const std::map<string, string> vsps = defJson["vsp"];
	for (const auto&[key, value] : vsps) {
		LOG( key << " = " << value << "; " );

		auto tileset = new TileSet(get_path_relative(inputFile, value));

		this->tilesets[key] = tileset;
	}
	
	if( layersArray.size() != layersBulkArray.size() )
	{
		LOG( "layersCount (" << layersArray.size() << ") did not match (" << layersBulkArray.size() << ")" );
		exit(2);
	}
	else
	{		
		for( int i=0; i< layersArray.size(); i++ )
		{
			set_window_title(layersArray[i]["name"]);
			
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
			LOG("skipping setting layer " << item << " for now...");
		}
	}
}

void TileMap::draw( SDL_Rect draw_area, SDL_Surface* target, SDL_Rect targetRect )
{
	if (
		engine_options.naive_caching &&
		draw_area.x == this->cachedLocation.x &&
		draw_area.y == this->cachedLocation.y &&
		draw_area.h == this->cachedLocation.h &&
		draw_area.w == this->cachedLocation.w 
	)
	{
		SDL_BlitSurface(this->cachedBuffer, &targetRect, target, &targetRect);
		return;
	}
	
	clock_t beginFrame = clock();
	for( auto curLayer: this->orderedLayers )
	{
		curLayer->draw(draw_area, target, targetRect);
	}

	cout << "Draw Area " << draw_area.x << "," << draw_area.y << "," << draw_area.h << "," << draw_area.w << endl;
	cout << "Target Rect " << targetRect.x << "," << targetRect.y << "," << targetRect.h << "," << targetRect.w << endl;
	
	clock_t endFrame = clock();
	// LOG( "full map render took " << endFrame - beginFrame << "ms" );

	if(engine_options.naive_caching) {
		cache(draw_area, target, targetRect);
	}
}


void TileMap::cache(SDL_Rect draw_area, SDL_Surface* target, SDL_Rect targetRect)
{
	this->cachedLocation = draw_area;

	Uint32 rmask, gmask, bmask, amask;
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		int shift = (req_format == STBI_rgb) ? 8 : 0;
		rmask = 0xff000000 >> shift;
		gmask = 0x00ff0000 >> shift;
		bmask = 0x0000ff00 >> shift;
		amask = 0x000000ff >> shift;
	#else // little endian, like x86
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
	#endif

	int depth = 32;
	int pitch = 4 * target->w;

	if(cachedBuffer == nullptr)
	{
		this->cachedBuffer = SDL_CreateRGBSurface(0, target->w, target->h, depth, rmask, gmask, bmask, amask);
	}

	cout << "SNAP! " << target->w << "," << target->h << endl;
	cout << "FROM! " << targetRect.w << "," << targetRect.h << endl;
	cout << "(from x,y) " << targetRect.x << "," << targetRect.y << endl;
	
	SDL_BlitSurface(target, &targetRect, this->cachedBuffer, &targetRect);
	//if(SDL_SaveBMP(this->cachedBuffer, "c:/tmp/frame.bmp") != 0)
	//{
	//	// Error saving bitmap
	//	printf("SDL_SaveBMP failed: %s\n", SDL_GetError());
	//}
}
