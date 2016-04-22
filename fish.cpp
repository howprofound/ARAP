#include "fish.h"

Fish::Fish()
{
	texture = NULL;
	x = SCREEN_WIDTH / 2;
	y = SCREEN_HEIGHT - 30;
}
Fish::Fish(const char path[],int pozx, int pozy)
{
	texture = SDL_LoadBMP(path);
	x = pozx;
	y = pozy;
	angle = 0;
}

SDL_Surface* Fish::getTexture()
{
	return this->texture;
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


void Fish::setText(SDL_Renderer* renderer) {
	SDL_SetColorKey(this->texture, true, 0x000000);
	this->text = SDL_CreateTextureFromSurface(renderer, this->texture);
}

SDL_Texture *Fish::getText() {
	return this->text;
}