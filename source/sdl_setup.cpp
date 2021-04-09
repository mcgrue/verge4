#include <SDL.h>
#include <iostream>

#include "sdl_setup.h"
#include "tileset.h"
#include "tilemap.h"

#include "util/util.h"

using namespace std;

SDL_Surface* m_guy;
SDL_Surface* m_tileset;
SDL_Rect     m_guy_position, m_map_position;
double       m_image_x;
double       m_image_y;

SDL_Window*   m_window;
SDL_Surface*  m_window_surface;
SDL_Event     m_window_event;
SDL_Renderer* m_renderer;

Direction    m_direction;

TileSet		 vsp;
TileMap		 level;

SDL_Window* get_game_window()
{
	return m_window;
}

SDL_Renderer* get_game_renderer()
{
	return m_renderer;
}

void draw() {
	SDL_FillRect(m_window_surface, nullptr, SDL_MapRGB(m_window_surface->format, 0, 0, 0));

	level.draw({ 0,0,640,480 }, m_window_surface, { 0,0,640,480 });
	
	SDL_BlitSurface(m_guy, nullptr, m_window_surface, &m_guy_position);
	SDL_UpdateWindowSurface(m_window);
}

void update(double delta_time) {

	double d = (5 * delta_time);
	
	switch (m_direction) {
		case Direction::NONE:
			break;
		case Direction::UP:
			m_image_y = m_image_y - d;
			break;
		case Direction::DOWN:
			m_image_y = m_image_y + d;
			break;
		case Direction::LEFT:
			m_image_x = m_image_x - d;
			break;
		case Direction::RIGHT:
			m_image_x = m_image_x + d;
			break;
		default:
			break;
	}

	m_guy_position.x = (int)m_image_x;
	m_guy_position.y = (int)m_image_y;
}

void init() {
	m_window = SDL_CreateWindow("SDL2 Window",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		680, 480,
		0);

	if (!m_window)
	{
		std::cout << "Failed to create window\n";
		std::cout << "SDL2 Error: " << SDL_GetError() << "\n";
		return;
	}

	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_renderer == nullptr) {
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		return;
	}

	m_window_surface = SDL_GetWindowSurface(m_window);

	if (!m_window_surface)
	{
		std::cout << "Failed to get window's surface\n";
		std::cout << "SDL2 Error: " << SDL_GetError() << "\n";
		return;
	}

	m_guy = load_surface("assets/img/stick_figure.bmp");
	// m_tileset = load_surface("assets/img/evil_lab.tiles.png");

	level = TileMap("assets/maps/evil_lab_f1.map.json");

	m_guy_position.x = 0;
	m_guy_position.y = 0;
	m_guy_position.w = 22;
	m_guy_position.h = 43;

	m_map_position.x = 0;
	m_map_position.y = 0;
	m_map_position.w = 320;
	m_map_position.h = 1008;

	m_image_x = 0.0;
	m_image_y = 0.0;
}

void destroy()
{
	SDL_FreeSurface(m_window_surface);
	SDL_DestroyWindow(m_window);
}

void update_input(bool& keep_window_open)
{
	while (SDL_PollEvent(&m_window_event) > 0)
	{
		switch (m_window_event.type)
		{
		case SDL_QUIT:
			keep_window_open = false;
			break;
		case SDL_KEYDOWN:
			{
				Uint8 const *keys = SDL_GetKeyboardState(nullptr);

				if (keys[SDL_SCANCODE_W] == 1 || keys[SDL_SCANCODE_UP] == 1) {
					m_direction = Direction::UP;
				} 
				else if (keys[SDL_SCANCODE_S] == 1 || keys[SDL_SCANCODE_DOWN] == 1) {
					m_direction = Direction::DOWN;
				} 
				else if (keys[SDL_SCANCODE_A] == 1 || keys[SDL_SCANCODE_LEFT] == 1) {
					m_direction = Direction::LEFT;
				} 
				else if (keys[SDL_SCANCODE_D] == 1 || keys[SDL_SCANCODE_RIGHT] == 1) {
					m_direction = Direction::RIGHT;
				}
			}
			break;
		case SDL_KEYUP:
			m_direction = Direction::NONE;
			break;
		}
	}
}

void loop() {
	bool keep_window_open = true;
	while (keep_window_open)
	{
		update_input(keep_window_open);

		update(1.0 / 60.0);
		draw();
	}
}
