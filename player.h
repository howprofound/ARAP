#pragma once
#include <iostream>
#include "./sdl/include/SDL.h"
#include "./sdl/include/SDL_main.h"
#include "fish.h"


class Player
{
private:
	int number;
	int points;
	Fish *fish;

public:
	Player();
	Player(int, int, int, int);
	int getNumber();
	void setPoints(int);
	int getPoints();
	void setFish(Fish*);
	Fish* getFish();
};