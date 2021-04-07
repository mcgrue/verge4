#ifndef SDL_SETUP_H
#define SDL_SETUP_H

#include "SDL.h"

enum class Direction
{
	NONE,
	UP,
	DOWN,
	LEFT,
	RIGHT
};

void draw();
void update(double delta_time);
SDL_Surface *load_bmp(char const *path);
void init();
void destroy();
void loop();

#endif //SDL_SETUP_H