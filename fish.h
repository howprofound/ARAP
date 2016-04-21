#pragma once
#include "./sdl/include/SDL.h"
#include "./sdl/include/SDL_main.h"

#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	600

class Fish
{
private:
	SDL_Surface* texture;
	SDL_Texture* text;
	float x, y;

public:
	Fish();
	Fish(const char path[],int,int);
	SDL_Surface* getTexture();
	SDL_Texture* getText();
	void setText(SDL_Renderer*);
	void setX(float x);
	float getX();
	void setY(float y);
	float getY();

	int angle;
};