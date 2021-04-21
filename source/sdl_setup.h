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
void init();
void destroy();
void loop();
SDL_Window* get_game_window();
SDL_Renderer* get_game_renderer();
SDL_Texture* get_current_screen_as_texture();