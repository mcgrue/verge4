#include "camera.h"

#include <iostream>

using namespace std;

pair<SDL_Rect, SDL_Rect> _no_wrap(const SDL_Rect& player, const SDL_Rect& camera, const SDL_Rect& screen, TileMap* map)
{
	auto new_camera = camera;
	auto new_player = player;

	const auto half_screen_w = screen.w / 2;
	const auto half_screen_h = screen.h / 2;
	
	if (player.x < half_screen_w)
	{
		new_camera.x = 0;
	}
	else if (player.x > map->size().w - half_screen_w)
	{
		new_camera.x = map->size().w - screen.w;
		new_player.x = player.x - new_camera.x;
	}
	else
	{
		new_camera.x = player.x - half_screen_w;
		new_player.x = half_screen_w;
	}

	if (player.y < half_screen_h)
	{
		new_camera.y = 0;
	}
	else if (player.y > map->size().h - half_screen_h)
	{
		new_camera.y = map->size().h - screen.h;
		new_player.y = player.y - new_camera.y;
	}
	else
	{
		new_camera.y = player.y - half_screen_h;
		new_player.y = half_screen_h;
	}
	
	return pair<SDL_Rect, SDL_Rect>(new_camera,new_player);
}

/// TODO: pass mode/mapsize through to make it easier to test
pair<SDL_Rect, SDL_Rect> clamp_camera(const SDL_Rect& mapwise_player, const SDL_Rect& camera, const SDL_Rect& screen, TileMap* map)
{
	switch( map->wrap_mode() )
	{
		case NO_WRAP:
			return _no_wrap(mapwise_player, camera, screen, map);
		default:
			cout << "WRAP MODE UNSUPPORTED" << endl;
			exit(2);
	}
}

