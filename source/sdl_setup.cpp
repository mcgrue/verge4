#include "SDL.h"
#include <iostream>

#include "sdl_setup.h"

using namespace std;

SDL_Surface *m_image;
SDL_Rect     m_image_position;
double       m_image_x;
double       m_image_y;

SDL_Window  *m_window;
SDL_Surface *m_window_surface;
SDL_Event    m_window_event;

Direction    m_direction;

void draw() {
	SDL_FillRect(m_window_surface, NULL, SDL_MapRGB(m_window_surface->format, 0, 0, 0));
	SDL_BlitSurface(m_image, NULL, m_window_surface, &m_image_position);
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

	m_image_position.x = (int)m_image_x;
	m_image_position.y = (int)m_image_y;
}

SDL_Surface *load_surface(char const *path) {
	SDL_Surface *image_surface = SDL_LoadBMP(path);

	if (!image_surface)
		return 0;

	return image_surface;
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

	m_image = load_surface("assets/img/stick_figure.bmp");

	m_image_position.x = 0;
	m_image_position.y = 0;
	m_image_position.w = 22;
	m_image_position.h = 43;

	m_image_x = 0.0;
	m_image_y = 0.0;
}

void destroy()
{
	SDL_FreeSurface(m_window_surface);
	SDL_DestroyWindow(m_window);
}

void loop() {
	bool keep_window_open = true;
	while (keep_window_open)
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

					if (keys[SDL_SCANCODE_W] == 1) {
						cout << "up" << endl;
						m_direction = Direction::UP;
					} 
					else if (keys[SDL_SCANCODE_S] == 1) {
						cout << "down" << endl;
						m_direction = Direction::DOWN;
					} 
					else if (keys[SDL_SCANCODE_A] == 1) {
						cout << "left" << endl;
						m_direction = Direction::LEFT;
					} 
					else if (keys[SDL_SCANCODE_D] == 1) {
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

		update(1.0 / 60.0);
		draw();
	}
}
