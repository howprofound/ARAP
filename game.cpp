#include "game.h"

Game::Game()
{
	this->quit = false;
	this->started = false;
}

void Game::setStatus(bool quit)
{
	this->quit = quit;
}

bool Game::getStatus()
{
	return this->quit;
}

void Game::setIsStarted(bool started)
{
	this->started = started;
}

bool Game::isStarted()
{
	return this->started;
}