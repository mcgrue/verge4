using namespace std;

#include <sdl.h>
#include "tileset.h"
#include "util/util.h"

TileSet::TileSet()
{
	this->srcImg = nullptr;
}

TileSet::TileSet(const string& inputFile)
{
	const auto json = get_json_from_file(inputFile);
	this->data = json;
	this->pathToJSON = inputFile;
	const auto source_image = json["source_image"];
	std::string blah = source_image;
	const auto pathToTilesetImage = get_path_relative(inputFile, blah);

	assert_file_exists(pathToTilesetImage);
	this->absPathToSrcImg = pathToTilesetImage;
	this->srcImg = load_texture(this->absPathToSrcImg);
	

	this->src.h = this->dest.h = this->tilesize.height = json["tilesize"]["height"];
	this->src.w = this->dest.w = this->tilesize.width = json["tilesize"]["width"];

	this->tilesPerRow = json["tiles_per_row"];
}

TileSet& TileSet::operator=(TileSet& o) noexcept // copy
{
	this->srcJSONPath = o.srcJSONPath;
	this->srcImg = o.srcImg; // do we want to copy the actual texture so we have a new instance? 😔
	this->data = o.data;
	this->pathToJSON = o.pathToJSON;
	this->tilesize = o.tilesize;
	this->tilesPerRow = o.tilesPerRow;
	this->dest = o.dest;
	this->src = o.src;

	return *this;
}

TileSet& TileSet::operator=(TileSet&& other) noexcept // move
{
	std::swap(srcJSONPath, other.srcJSONPath); // exchange resources between *this and other
	std::swap(srcImg, other.srcImg);
	std::swap(data, other.data);
	std::swap(pathToJSON, other.pathToJSON);
	std::swap(tilesize, other.tilesize);
	std::swap(tilesPerRow, other.tilesPerRow);
	std::swap(dest, other.dest);
	std::swap(src, other.src);
	return *this;
}

inline TileSet::TileSet(TileSet&& o) noexcept
{
	this->srcJSONPath = std::move(o.srcJSONPath);
	this->srcImg = std::move(o.srcImg);
	this->data = std::move(o.data);
	this->pathToJSON = std::move(o.pathToJSON);
	this->tilesize = std::move(o.tilesize);
	this->tilesPerRow = std::move(o.tilesPerRow);
	this->dest = std::move(o.dest);
	this->src = std::move(o.src);
}

void TileSet::drawTile(SDL_Renderer* renderer, tileindex_t idx, pixelcoordinates_t drawTo)
{
	if (idx == 0 && engine_options.never_render_tile_0)
	{
		return;
	}

	src.x = (idx % this->tilesPerRow) * src.w;
	src.y = (idx / this->tilesPerRow) * src.h;

	dest.x = drawTo.x;
	dest.y = drawTo.y;

	
	SDL_RenderCopy(renderer, this->srcImg, &src, &dest);
}

SDL_Texture* TileSet::getSourceImage() const
{
	return this->srcImg;
}
