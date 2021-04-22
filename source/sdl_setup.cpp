#include <SDL.h>
#include <iostream>
#include <sstream>

#include "sdl_setup.h"
#include "tileset.h"
#include "tilemap.h"

#include "util/util.h"

using namespace std;

SDL_Texture* player_texture;
SDL_Rect player_size = {0,0,22,43};
SDL_Rect player_position = { 260, 180,22,43 };
SDL_Rect player_previous_position = player_position;
subpixel_coordinates_t player_location = { 260, 180 };
Direction    player_direction;

bool guy_is_moving = false;

SDL_Window*   m_window;
SDL_Event     m_window_event;
SDL_Renderer* m_renderer;

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

int cur_screen_w = engine_options.initial_window_size.w;
int cur_screen_h = engine_options.initial_window_size.h;

SDL_Texture* get_current_screen_as_texture() {
	return SDL_CreateTexture(m_renderer,
		SDL_PIXELFORMAT_BGRA8888,
		SDL_TEXTUREACCESS_STREAMING,
		cur_screen_w,
		cur_screen_h);
}


Uint8 cur_bg_r = std::to_integer<Uint8>(engine_options.default_background_color.r);
Uint8 cur_bg_g = std::to_integer<Uint8>(engine_options.default_background_color.g);
Uint8 cur_bg_b = std::to_integer<Uint8>(engine_options.default_background_color.b);

SDL_Rect screen_rect = { 0,0, cur_screen_w, cur_screen_h };
SDL_Rect camera_rect = { 0,0, cur_screen_w, cur_screen_h };

void draw() {
	SDL_SetRenderDrawColor(m_renderer, cur_bg_r, cur_bg_g, cur_bg_b, 1);
	SDL_RenderFillRect(m_renderer, &screen_rect);

	level.draw(m_renderer, camera_rect, screen_rect);

	SDL_RenderCopy(m_renderer, player_texture, &player_size, &player_position);

	SDL_RenderPresent(m_renderer);
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
	
	switch (player_direction) {
		case Direction::NONE:
			break;
		case Direction::UP:
			player_location.y -= d;
			break;
		case Direction::DOWN:
			player_location.y += d;
			break;
		case Direction::LEFT:
			player_location.x -= d;
			break;
		case Direction::RIGHT:
			player_location.x += d;
			break;
		default:
			break;
	}

	if( player_previous_position.x != player_position.x || player_previous_position.y != player_position.y )
	{
		guy_is_moving = true;
		player_previous_position.x = player_position.x;
		player_previous_position.y = player_position.y;

		cout << "GUY MOVED TO " << player_position.x << "," << player_position.y << endl;
	} else
	{
		guy_is_moving = false;
	}
	

	player_position.x = (int)player_location.x;
	player_position.y = (int)player_location.y;
}

void init() {

	// just in case we want to use renderdoc...
	int preferredDriver = -1;
	int nDrivers = SDL_GetNumRenderDrivers();
	for (int i = 0; i < nDrivers; i++)
	{
		SDL_RendererInfo info;
		SDL_GetRenderDriverInfo(i, &info);
		if (!strcmp(info.name, "opengles2"))
			preferredDriver = i;
	}
	
	m_window = SDL_CreateWindow("VERGE 4: The World's Most Extraneous 2d C++ Game Engine",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		cur_screen_w, cur_screen_h,
		0);

	if (!m_window)
	{
		LOG("Failed to create window \n SDL2 Error: " << SDL_GetError() );
		return;
	}

	m_renderer = SDL_CreateRenderer(m_window, preferredDriver, SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr) {
		LOG( "SDL_CreateRenderer Error: " << SDL_GetError() );
		return;
	}

	SDL_SetWindowTitle(m_window, "Loading map...");
	
	player_texture = load_texture("assets/img/stick_figure.bmp");

	level = TileMap("assets/maps/evil_lab_f1.map.json");
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
					player_direction = Direction::UP;
				} 
				else if (keys[SDL_SCANCODE_S] == 1 || keys[SDL_SCANCODE_DOWN] == 1) {
					player_direction = Direction::DOWN;
				} 
				else if (keys[SDL_SCANCODE_A] == 1 || keys[SDL_SCANCODE_LEFT] == 1) {
					player_direction = Direction::LEFT;
				} 
				else if (keys[SDL_SCANCODE_D] == 1 || keys[SDL_SCANCODE_RIGHT] == 1) {
					player_direction = Direction::RIGHT;
				}
			}
			break;
		case SDL_KEYUP:
			player_direction = Direction::NONE;
			break;
		}
	}
}

void update_fps_counter(clock_t& deltaTime, unsigned& frames, double& frameRate, double averageFrameTimeMilliseconds, const bool vsync)
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
