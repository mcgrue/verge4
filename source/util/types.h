#pragma once

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

union vec2d {
	struct {
		double x, y;
	};
	struct {
		double w, h;
	};
	double m[2];
};

typedef vec2i tilesize_t;
typedef vec2d subpixel_coordinates_t;
typedef vec2i pixelcoordinates_t;
typedef vec2i screen_size_t;