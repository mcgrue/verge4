#include "tileanimation.h"

#include <utility>

TileAnimation::TileAnimation(const string& name, int start, int end, int delay, int mode)
{
	this->name = name;
	this->start = start;
	this->end = end;
	this->delay = delay;
	this->mode = static_cast<Tile_Animation_Mode>(mode);
}

TileAnimation::TileAnimation(const string& name, int start, int end, int delay, Tile_Animation_Mode mode)
{
	this->name = name;
	this->start = start;
	this->end = end;
	this->delay = delay;
	this->mode = mode;
}