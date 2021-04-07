#pragma once

#include <filesystem>
#include <sdl.h>
#include <string>
#include "third_party/JSON/json.hpp"
#include "util/util.h"

using namespace std;
using namespace nlohmann;

class TileSet {

public:
	TileSet();
	TileSet(const string& inputFile); // simple constructor
	TileSet(const TileSet& obj); // copy constructor
	TileSet& operator=(TileSet other) noexcept;
	TileSet(TileSet&& o) noexcept; // move constructor
	~TileSet(); // destructor

	SDL_Surface* getSourceImage();

private:
	string srcJSONPath;
	
	SDL_Surface* srcImg;
	string absPathToSrcImg;
	basic_json<> data;
	string pathToJSON;
};

inline TileSet::TileSet()
{
	this->srcImg = nullptr;
}

inline TileSet::TileSet(const string& inputFile)
{
	const auto json = get_json_from_file(inputFile);
	this->data = json;
	this->pathToJSON = inputFile;
	const auto source_image = json["source_image"];
	std::string blah = source_image;
	const auto dickbutts = get_path_relative(inputFile, blah);

	assert_file_exists(dickbutts);
	this->absPathToSrcImg = dickbutts;
	this->srcImg = load_surface(this->absPathToSrcImg);
}

inline TileSet::TileSet(const TileSet& obj)
{
	
}

inline TileSet& TileSet::operator=(TileSet other) noexcept // call copy or move constructor to construct other
{
	std::swap(srcJSONPath, other.srcJSONPath); // exchange resources between *this and other
	std::swap(srcImg, other.srcImg);
	std::swap(data, other.data);
	std::swap(pathToJSON, other.pathToJSON);
	
	return *this;
}

inline TileSet::TileSet(TileSet&& o) noexcept
{
	this->srcJSONPath = std::move(o.srcJSONPath);
	this->srcImg = std::move(o.srcImg);
	this->data = std::move(o.data);
	this->pathToJSON = std::move(o.pathToJSON);
}

inline TileSet::~TileSet()
{
	
}

inline SDL_Surface* TileSet::getSourceImage()
{
	return this->srcImg;
}

