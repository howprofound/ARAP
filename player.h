#pragma once
#include <iostream>
#include "./sdl/include/SDL.h"
#include "./sdl/include/SDL_main.h"
#include "fish.h"


class Player
{
public:
	Player();
	Player(int, int, int);
	int getNumber();
	void setPoints(int);
	int getPoints();

	Fish *fish;
private:
	int number;
	int points;





};