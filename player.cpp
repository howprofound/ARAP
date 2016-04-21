#include "player.h"

Player::Player()
{
	this->number = 0;
	this->fish = NULL;
	this->server = new Server();
	this->menuIndex = 0;
	this->menuPosition = 0;
}

Player::Player(int number, int x, int y)
{
	this->number = number;
	this->fish = new Fish("./images/ryba1.bmp", x, y);
	this->server = new Server();
	this->menuIndex = 0;
	this->menuPosition = 0;
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

void Player::setMenuIndex(int menuIndex)
{
	this->menuIndex = menuIndex;
}

int Player::getMenuIndex()
{
	return this->menuIndex;
}

void Player::setMenuPosition(int menuPosition)
{
	this->menuPosition = menuPosition;
}

int Player::getMenuPosition()
{
	return this->menuPosition;
}