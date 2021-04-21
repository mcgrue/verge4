#include <SDL.h>
#include <iostream>
#include <sstream>

#include "sdl_setup.h"
#include "tileset.h"
#include "tilemap.h"

#include "util/util.h"

using namespace std;

SDL_Texture* m_guy;
SDL_Rect m_guy_rect = { 0,0,22,43 };

SDL_Rect     m_guy_previous_position, m_guy_position, m_map_position;
double       m_image_x;
double       m_image_y;

bool guy_is_moving = false;

SDL_Window*   m_window;
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

SDL_Texture* get_current_screen_as_texture() {
	return SDL_CreateTexture(m_renderer,
		SDL_PIXELFORMAT_BGRA8888,
		SDL_TEXTUREACCESS_STREAMING,
		640,
		480);
}


Uint8 cur_bg_r = std::to_integer<Uint8>(engine_options.default_background_color.r);
Uint8 cur_bg_g = std::to_integer<Uint8>(engine_options.default_background_color.g);
Uint8 cur_bg_b = std::to_integer<Uint8>(engine_options.default_background_color.b);

SDL_Rect screen_rect = { 0,0,640,480 };

void draw() {
	SDL_SetRenderDrawColor(m_renderer, cur_bg_r, cur_bg_g, cur_bg_b, 1);
	SDL_RenderFillRect(m_renderer, &screen_rect);

	level.draw(m_renderer, { (int)m_guy_position.x,(int)m_guy_position.y,640,480 },  { 0,0,640,480 });

	SDL_RenderCopy(m_renderer, m_guy, &m_guy_rect, &m_guy_position);

	SDL_RenderPresent(m_renderer);
	// SDL_UpdateWindowSurface(m_window);
}

double wait_a_second = 0;

void update(double delta_time) {

	double d = (5 * delta_time);

	if(engine_options.pixel_movement_debug) {
		wait_a_second += delta_time;

		if (wait_a_second < 10) {
			// cout << "DED" << wait_a_second << endl;
			return;
		}

		wait_a_second = 0;
		d = 1;
	}
	
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

	if( m_guy_previous_position.x != m_guy_position.x || m_guy_previous_position.y != m_guy_position.y )
	{
		guy_is_moving = true;
		m_guy_previous_position.x = m_guy_position.x;
		m_guy_previous_position.y = m_guy_position.y;

		cout << "GUY MOVED TO " << m_guy_position.x << "," << m_guy_position.y << endl;
	} else
	{
		guy_is_moving = false;
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

	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr) {
		LOG( "SDL_CreateRenderer Error: " << SDL_GetError() );
		return;
	}

	SDL_SetWindowTitle(m_window, "Loading map...");
	
	m_guy = load_texture("assets/img/stick_figure.bmp");

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
		// averageFrameTimeMilliseconds = 1000.0 / (frameRate == 0 ? 0.001 : frameRate);

		/*
		if (vsync)
			LOG( "FrameTime was:" << averageFrameTimeMilliseconds );
		else
			LOG( "CPU time was:" << averageFrameTimeMilliseconds );
		*/

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
		// LOG( "full update loop took " << endFrame - beginFrame << "ms" );

		frameTime = endFrame - beginFrame;
		deltaTime += (int)frameTime;
		frames++;
		
		update_fps_counter(deltaTime, frames, frameRate, averageFrameTimeMilliseconds, vsync);
	}
}
