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
	void setMenuIndex(int);
	int getMenuIndex();
	void setMenuPosition(int);
	int getMenuPosition();

	Fish *fish;
	Server *server;
	char ip[16];
	int cursorPosition;
private:
	int number;
	int points;
	int menuIndex;
	int menuPosition;





};
