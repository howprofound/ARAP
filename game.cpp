#include "game.h"

Game::Game()
{
	quit = false;
}

void Game::setStatus(bool quit)
{
	this->quit = quit;
}

bool Game::getStatus()
{
	return this->quit;
}