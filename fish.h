#pragma once
#include "./sdl/include/SDL.h"
#include "./sdl/include/SDL_main.h"

#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	600

class Fish
{
private:
	SDL_Surface* surface;
	SDL_Texture* texture;
	float x, y;
	int angle;
	float back;
	float predator;

public:
	Fish();
	Fish(const char path[], int, int);
	SDL_Texture* getTexture();
	void setTexture(SDL_Renderer*);
	void setX(float);
	float getX();
	void setY(float);
	float getY();
	void setAngle(int);
	int getAngle();
	void setBack(float);
	float getBack();
	void setPredator(float);
	float getPredator();
};