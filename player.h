#pragma once
#include <iostream>
#include "./sdl/include/SDL.h"
#include "./sdl/include/SDL_main.h"
#include "fish.h"
#include "server.h"

class Player
{
public:
	Player();
	Player(int, int, int);
	int getNumber();
	void setPoints(int);
	int getPoints();

	Fish *fish;
	Server *server;
private:
	int number;
	int points;





};
