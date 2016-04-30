#include "fish.h"

Fish::Fish()
{
	surface = NULL;
	x = SCREEN_WIDTH / 2;
	y = SCREEN_HEIGHT / 2;
	angle = 0;
	back = 0;
	predatorAngle = 0;
}

Fish::Fish(const char path[], int pozx, int pozy, int angle)
{
	surface = SDL_LoadBMP(path);
	x = static_cast<float>(pozx);
	y = static_cast<float>(pozy);
	this->angle = angle;
	back = 0;
	predatorAngle = 0;
}

void Fish::setX(float x)
{
	this->x = x;
}

float Fish::getX()
{
	return this->x;
}

void Fish::setY(float y)
{
	this->y = y;
}

float Fish::getY()
{
	return this->y;
}

void Fish::setTexture(SDL_Renderer* renderer)
{
	SDL_SetColorKey(this->surface, true, 0x000000);
	this->texture = SDL_CreateTextureFromSurface(renderer, this->surface);
}

SDL_Texture *Fish::getTexture()
{
	return this->texture;
}

void Fish::setAngle(int angle)
{
	this->angle = angle;
}

int Fish::getAngle()
{
	return this->angle;
}

void Fish::setBack(float back)
{
	this->back = back;
}

float Fish::getBack()
{
	return this->back;
}

void Fish::setPredatorAngle(int predatorAngle)
{
	this->predatorAngle = predatorAngle;
}

int Fish::getPredatorAngle()
{
	return this->predatorAngle;
}