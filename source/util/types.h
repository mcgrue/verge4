#pragma once

struct TileSize
{
public:
	int width;
	int height;
};

typedef unsigned int tileindex_t;

union vec2i {
	struct {
		int x, y;
	};
	struct {
		int w, h;
	};
	int m[2];
};

typedef vec2i pixelcoordinates_t;
