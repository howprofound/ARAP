#include "player.h"

Player::Player()
{
	this->number = 0;
	this->fish = NULL;
	this->points = 0;
	this->back = 0;
}

Player::Player(int number, int x, int y, int angle)
{
	this->number = number;
	char path[128];
	sprintf_s(path, "./images/ryba%d.bmp", number + 1);
	this->fish = new Fish(path, x, y, angle);
	this->points = 0;
	this->back = 0;
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

void Player::setFish(Fish* fish)
{
	this->fish = fish;
}

Fish* Player::getFish()
{
	return this->fish;
}

void Player::setBack(int back)
{
	this->back = back;
}

int Player::getBack()
{
	return this->back;
}