#include <iostream>

#include "SDL.h"
#include "third_party/JSON/json.hpp"

#include "sdl_setup.h"

using namespace std;
using json = nlohmann::json;

int SDL_main(int argc, char **argv) {
	auto j3 = json::parse(R"({"happy": true, "pi": 3.141})");

	cout << j3["pi"] << SDL_atoi("a") << endl;

	init();
	loop();

	return 0;
}
