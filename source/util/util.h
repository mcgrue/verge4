#pragma once

#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <SDL_image.h>
#include <cerrno>
#include "third_party/JSON/json.hpp"
#include "sdl_setup.h"

struct engine_options
{
	bool never_render_tile_0 = true;
	bool naive_caching = false;
};

inline engine_options engine_options;

namespace fs = std::experimental::filesystem;
using namespace std;
using namespace nlohmann;

inline void assert_file_exists(const string& inputFile)
{
	const auto path = fs::path(inputFile);
	if (!exists(path)) {
		throw std::invalid_argument("File not found: '" + inputFile + "'");
	}
};

/// fastest as per https://insanecoding.blogspot.com/2011/11/how-to-read-in-file-in-c.html
inline std::string get_file_contents(const std::string& filename)
{
	assert_file_exists(filename);
	
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

inline basic_json<> get_json_from_file(const std::string& filename)
{
	return json::parse(get_file_contents(filename));
}

inline SDL_Surface* load_surface(const std::string& path)
{
	//The final optimized image
	SDL_Surface* optimizedSurface = nullptr;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == nullptr)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Convert surface to screen format
		optimizedSurface = SDL_ConvertSurface(loadedSurface, SDL_GetWindowSurface(get_game_window())->format, 0);
		if (optimizedSurface == nullptr)
		{
			printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return optimizedSurface;
}

inline SDL_Texture* load_texture(const std::string& path)
{
	return IMG_LoadTexture(get_game_renderer(), path.c_str());
}

inline fs::path get_my_directory(const string& filepath)
{
	auto p = fs::path(filepath);

	if(is_directory(p))
	{
		return p;
	}
	
	return p.parent_path();
}

inline std::string get_path_relative(const fs::path& base, const fs::path& rel)
{
	fs::path absBase;
	
	if(is_directory(base))
	{
		absBase = fs::absolute(rel, base);
	} else
	{
		absBase = fs::absolute(rel, base.parent_path());
	}
		
	return absBase.string();
}

inline std::string get_path_relative(const std::string& base, const fs::path& rel)
{
	return get_path_relative(fs::path(base), rel);
}

inline std::string get_path_relative(const fs::path& base, const std::string& rel)
{
	return get_path_relative(base, fs::path(rel));
}

inline std::string get_path_relative(const std::string& base, const std::string& rel)
{
	return get_path_relative(fs::path(base), fs::path(rel));
}


inline int getFlatIdx(int x, int y, int numColumns) {
	return numColumns * y + x;
};

// extracts the first dimension of a flat-indexed 2 dimensionally array given
// the second dimension's maximum value and the value of the flat index you
// wish to extract the first dimension's value from.
//
inline int getXfromFlat(int idx, int numColumns) {
	return idx % numColumns;
};

// extracts the second dimension of a flat-indexed 2 dimensionally array given
// the second dimension's maximum value and the value of the flat index you
// wish to extract the second dimension's value from.
//
inline int getYfromFlat( int idx, int numColumns) {
	const int flatval = idx - getXfromFlat(idx, numColumns);
	return flatval / numColumns;
};

inline double clockToMilliseconds(clock_t ticks) {
	// units / (units / time) = > time(seconds) * 1000 = milliseconds
	return (ticks / (double)CLOCKS_PER_SEC)*1000.0;
}

inline void set_window_title(string str) {
	SDL_SetWindowTitle(get_game_window(), str.c_str());
};

#define LOG(f) \
    do { std::stringstream s; \
         s << f; \
         cout << s.str() << endl; \
    } while (0)

// log(__FILE__, __LINE__, __FUNCTION__, s);
