#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

#include "sdl_setup.h"

using namespace std;

SDL_Surface* m_guy;
SDL_Surface* m_tileset;
SDL_Rect     m_guy_position, m_map_position;
double       m_image_x;
double       m_image_y;

SDL_Window*  m_window;
SDL_Surface* m_window_surface;
SDL_Event    m_window_event;

Direction    m_direction;

void draw() {
	SDL_FillRect(m_window_surface, nullptr, SDL_MapRGB(m_window_surface->format, 0, 0, 0));
	SDL_BlitSurface(m_tileset, nullptr, m_window_surface, &m_map_position);
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

SDL_Surface* load_surface(std::string path)
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
		optimizedSurface = SDL_ConvertSurface(loadedSurface, SDL_GetWindowSurface(m_window)->format, 0);
		if (optimizedSurface == nullptr)
		{
			printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return optimizedSurface;
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

	m_window_surface = SDL_GetWindowSurface(m_window);

	if (!m_window_surface)
	{
		std::cout << "Failed to get window's surface\n";
		std::cout << "SDL2 Error: " << SDL_GetError() << "\n";
		return;
	}

	m_guy = load_surface("assets/img/stick_figure.bmp");
	m_tileset = load_surface("assets/img/evil_lab.tiles.png");

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
					cout << "up" << endl;
					m_direction = Direction::UP;
				} 
				else if (keys[SDL_SCANCODE_S] == 1 || keys[SDL_SCANCODE_DOWN] == 1) {
					cout << "down" << endl;
					m_direction = Direction::DOWN;
				} 
				else if (keys[SDL_SCANCODE_A] == 1 || keys[SDL_SCANCODE_LEFT] == 1) {
					cout << "left" << endl;
					m_direction = Direction::LEFT;
				} 
				else if (keys[SDL_SCANCODE_D] == 1 || keys[SDL_SCANCODE_RIGHT] == 1) {
					cout << "rigth" << endl;
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
