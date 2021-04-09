#include <iostream>
#include <Windows.h>
#include <SDL.h>

#include "sdl_setup.h"

#include "util/util.h"
#include "third_party/JSON/json.hpp"

using namespace std;
using json = nlohmann::json;

int SDL_main(int argc, char **argv) {
	auto j3 = json::parse(R"({"happy": true, "pi": 3.141})");

	LOG( j3["pi"] << SDL_atoi("a") );

	init();
	loop();

	return 0;
}
