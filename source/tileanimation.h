#pragma once

#include <string>

using namespace std;

enum Tile_Animation_Mode
{
	LEFT_TO_RIGHT_REPEAT = 0,
	RIGHT_TO_LEFT_REPEAT = 1,
	RANDOM = 2,
	LEFT_TO_RIGHT_PING_PONG = 3,
	RIGHT_TO_LEFT_PING_PONG = 4
};

class TileAnimation
{
private:
	string name;
	int start;
	int end;
	int delay;
	Tile_Animation_Mode mode;

public:
	TileAnimation() = default;
	TileAnimation(const string& name, int start, int end, int delay, int mode);
	TileAnimation(const string& name, int start, int end, int delay, Tile_Animation_Mode mode);
	~TileAnimation() = default;
};