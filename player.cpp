#include "player.h"

Player::Player() {
	this->number = 0;
	this->fish = NULL;
}

Player::Player(int number,int x, int y) {

	this->number = number;
	this->fish = new Fish("./images/ryba1.bmp",x,y);
	this->server = new Server();
}

int Player::getNumber() {
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