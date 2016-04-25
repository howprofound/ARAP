#include "player.h"

Player::Player()
{
	this->number = 0;
	this->fish = NULL;
	this->server = new Server();
	this->points = 0;
}

Player::Player(int number, int x, int y)
{
	this->number = number;
	char path[128];
	sprintf(path, "./images/ryba%d.bmp", number + 1);
	this->fish = new Fish(path, x, y);
	this->server = new Server();
	this->points = 0;
}

int Player::getNumber()
{
	return this->number;
}

void Player::setPoints(int points)
{
	this->points = points;
}

int Player::getPoints()
{
	return this->points;
}