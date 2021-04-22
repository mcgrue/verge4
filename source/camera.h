#pragma once

#include <sdl.h>
#include <utility>

#include "tilemap.h"

/// returns new_camera, new_screenwise_player
pair<SDL_Rect, SDL_Rect> clamp_camera( const SDL_Rect& mapwise_player, const SDL_Rect& camera, const SDL_Rect& screen, TileMap* map );
