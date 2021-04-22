using namespace std;

#include <sdl.h>
#include "tileset.h"
#include "util/util.h"

TileSet::TileSet()
{
	this->src_img = nullptr;
}

TileSet::TileSet(const string& inputFile)
{
	const auto json = get_json_from_file(inputFile);
	this->data = json;
	this->path_to_json = inputFile;
	const auto source_image = json["source_image"];
	std::string blah = source_image;
	const auto pathToTilesetImage = get_path_relative(inputFile, blah);

	assert_file_exists(pathToTilesetImage);
	this->abs_path_to_src_img = pathToTilesetImage;
	this->src_img = load_texture(this->abs_path_to_src_img);
	

	this->src.h = this->dest.h = this->_tile_size.h = json["tilesize"]["height"];
	this->src.w = this->dest.w = this->_tile_size.w = json["tilesize"]["width"];

	this->tiles_per_row = json["tiles_per_row"];
}

TileSet& TileSet::operator=(TileSet& o) noexcept // copy
{
	this->src_JSON_path = o.src_JSON_path;
	this->src_img = o.src_img; // do we want to copy the actual texture so we have a new instance? 😔
	this->data = o.data;
	this->path_to_json = o.path_to_json;
	this->_tile_size = o._tile_size;
	this->tiles_per_row = o.tiles_per_row;
	this->dest = o.dest;
	this->src = o.src;

	return *this;
}

TileSet& TileSet::operator=(TileSet&& other) noexcept // move
{
	std::swap(src_JSON_path, other.src_JSON_path); // exchange resources between *this and other
	std::swap(src_img, other.src_img);
	std::swap(data, other.data);
	std::swap(path_to_json, other.path_to_json);
	std::swap(_tile_size, other._tile_size);
	std::swap(tiles_per_row, other.tiles_per_row);
	std::swap(dest, other.dest);
	std::swap(src, other.src);
	return *this;
}

inline TileSet::TileSet(TileSet&& o) noexcept
{
	this->src_JSON_path = std::move(o.src_JSON_path);
	this->src_img = std::move(o.src_img);
	this->data = std::move(o.data);
	this->path_to_json = std::move(o.path_to_json);
	this->_tile_size = std::move(o._tile_size);
	this->tiles_per_row = std::move(o.tiles_per_row);
	this->dest = std::move(o.dest);
	this->src = std::move(o.src);
}

void TileSet::drawTile(SDL_Renderer* renderer, tileindex_t idx, pixelcoordinates_t drawTo)
{
	if (idx == 0 && engine_options.never_render_tile_0)
	{
		return;
	}

	src.x = (idx % this->tiles_per_row) * src.w;
	src.y = (idx / this->tiles_per_row) * src.h;

	dest.x = drawTo.x;
	dest.y = drawTo.y;

	
	SDL_RenderCopy(renderer, this->src_img, &src, &dest);
}

SDL_Texture* TileSet::getSourceImage() const
{
	return this->src_img;
}
