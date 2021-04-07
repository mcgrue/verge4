#pragma once

#include <SDL.h>

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
SDL_Surface* load_bmp(char const *path);
void init();
void destroy();
void loop();
SDL_Window* get_game_window();
SDL_Renderer* get_game_renderer();