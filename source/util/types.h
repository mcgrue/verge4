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

union vec2f {
	struct {
		float x, y;
	};
	struct {
		float w, h;
	};
	float m[2];
};

typedef vec2i pixelcoordinates_t;