#include <iostream>
#include "SDL.h"
#include "third_party/JSON/json.hpp"

using namespace std;
using json = nlohmann::json;

SDL_Surface *m_image;
SDL_Rect     m_image_position;
double       m_image_x;
double       m_image_y;

SDL_Window  *m_window;
SDL_Surface *m_window_surface;
SDL_Event    m_window_event;

void draw() {
	SDL_FillRect(m_window_surface, NULL, SDL_MapRGB(m_window_surface->format, 0, 0, 0));
	SDL_BlitSurface(m_image, NULL, m_window_surface, &m_image_position);
	SDL_UpdateWindowSurface(m_window);
}

void update(double delta_time) {
	m_image_x = m_image_x + (5 * delta_time);
	m_image_position.x = (int)m_image_x;
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
				Uint8 const *keys = SDL_GetKeyboardState(nullptr);

				if (keys[SDL_SCANCODE_W] == 1)
					cout << "up" << endl;
				else if (keys[SDL_SCANCODE_S] == 1)
					cout << "down" << endl;
				else if (keys[SDL_SCANCODE_A] == 1)
					cout << "left" << endl;
				else if (keys[SDL_SCANCODE_D] == 1)
					cout << "right" << endl;
				break;
			}
		}

		update(1.0 / 60.0);
		draw();
	}
}


int SDL_main(int argc, char **argv) {
	auto j3 = json::parse(R"({"happy": true, "pi": 3.141})");

	cout << j3["pi"] << SDL_atoi("a") << endl;

	init();
	loop();

	return 0;
}
