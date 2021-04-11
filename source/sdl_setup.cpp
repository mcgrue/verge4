#include <SDL.h>
#include <iostream>
#include <sstream>

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

	// level.draw({ 0,0,640,480 }, m_window_surface, { 0,0,640,480 });
	level.draw({ (int)m_guy_position.x,(int)m_guy_position.y,640,480 }, m_window_surface, { 0,0,640,480 });
	
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
	m_window = SDL_CreateWindow("VERGE 4: The World's Most Extraneous 2d C++ Game Engine",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		680, 480,
		0);

	if (!m_window)
	{
		LOG("Failed to create window \n SDL2 Error: " << SDL_GetError() );
		return;
	}

	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_renderer == nullptr) {
		LOG( "SDL_CreateRenderer Error: " << SDL_GetError() );
		return;
	}

	m_window_surface = SDL_GetWindowSurface(m_window);

	if (!m_window_surface)
	{
		LOG( "Failed to get window's surface \n SDL2 Error: " << SDL_GetError() );
		return;
	}

	SDL_SetSurfaceBlendMode(m_window_surface, SDL_BLENDMODE_BLEND);

	SDL_SetWindowTitle(m_window, "Loading map...");
	
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

void update_fps_counter(clock_t& deltaTime, unsigned& frames, double frameRate, double averageFrameTimeMilliseconds, const bool vsync)
{
	if (clockToMilliseconds(deltaTime) > 1000.0) { //every second
		frameRate = (double)frames*0.5 + frameRate * 0.5; //more stable
		frames = 0;
		deltaTime -= CLOCKS_PER_SEC;
		averageFrameTimeMilliseconds = 1000.0 / (frameRate == 0 ? 0.001 : frameRate);

		if (vsync)
			LOG( "FrameTime was:" << averageFrameTimeMilliseconds );
		else
			LOG( "CPU time was:" << averageFrameTimeMilliseconds );

		stringstream title;
		title << frameRate << " FPS";
			
		SDL_SetWindowTitle(m_window, title.str().c_str());
	}
}

void loop() {
	clock_t deltaTime = 0;
	unsigned int frames = 0;
	double  frameRate = 30;
	double frameTime = 1;
	double  averageFrameTimeMilliseconds = 33.333;
	const bool vsync = SDL_GetWindowFlags(m_window) & SDL_RENDERER_PRESENTVSYNC;
		
	bool keep_window_open = true;
	while (keep_window_open)
	{
		const clock_t beginFrame = clock();

		update_input(keep_window_open);

		update(frameTime / 60.0);
		draw();

		const clock_t endFrame = clock();
		LOG( "full update loop took " << endFrame - beginFrame << "ms" );

		frameTime = endFrame - beginFrame;
		deltaTime += (int)frameTime;
		frames++;
		
		update_fps_counter(deltaTime, frames, frameRate, averageFrameTimeMilliseconds, vsync);
	}
}
