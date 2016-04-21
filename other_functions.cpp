#include "functions.h"


// definiuje kolory u¿yte w grze
void SetColours(SDL_PixelFormat* format, Uint32* colours)
{
	colours[BLACK] = SDL_MapRGB(format, 0, 0, 0);
	colours[RED] = SDL_MapRGB(format, 255, 16, 16);
	colours[ORANGE] = SDL_MapRGB(format, 255, 150, 10);
	colours[YELLOW] = SDL_MapRGB(format, 240, 240, 0);
	colours[GREEN] = SDL_MapRGB(format, 0, 230, 0);
	colours[CYAN] = SDL_MapRGB(format, 0, 235, 210);
	colours[BLUE] = SDL_MapRGB(format, 15, 50, 210);
	colours[PINK] = SDL_MapRGB(format, 255, 40, 210);
	colours[WHITE] = SDL_MapRGB(format, 255, 255, 255);
	colours[GREY] = SDL_MapRGB(format, 150, 150, 150);
	colours[GREY2] = SDL_MapRGB(format, 100, 100, 100);
	colours[GREY3] = SDL_MapRGB(format, 50, 50, 50);
}

// zwalnia powierzchniê i zamyka okno
void FreeMemoryAndQuit(SDL_Surface* charset, SDL_Surface* screen, SDL_Texture* scrtex, SDL_Window* window, SDL_Renderer* renderer, Game* game, Fish* fish)
{
	delete game;
	delete fish;
	SDL_FreeSurface(charset);
	SDL_FreeSurface(screen);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

void MoveFish(Fish* fish, double &delta,int &angle)
{
	float x = fish->getX();
	float y = fish->getY();

	const Uint8* keystate = SDL_GetKeyboardState(NULL);

	
	if (keystate[SDL_SCANCODE_RIGHT] && keystate[SDL_SCANCODE_LEFT]) {}
	else if (keystate[SDL_SCANCODE_UP] && keystate[SDL_SCANCODE_LEFT])
	{
		fish->setY(y - static_cast<float>(FISH_VELOCITY*delta * sin(angle*M_PI / 180)));
		fish->setX(x - static_cast<float>(FISH_VELOCITY*delta * cos(angle*M_PI / 180)));
		angle -= int(200*delta);
	}
	else if (keystate[SDL_SCANCODE_UP] && keystate[SDL_SCANCODE_RIGHT])
	{
		fish->setY(y - static_cast<float>(FISH_VELOCITY*delta * sin(angle*M_PI / 180)));
		fish->setX(x - static_cast<float>(FISH_VELOCITY*delta * cos(angle*M_PI / 180)));
		angle += int(200*delta);
	}


	else if (keystate[SDL_SCANCODE_LEFT])
	{
		angle -= int(200*delta);
	}
	else if (keystate[SDL_SCANCODE_RIGHT])
	{
		angle += int(200*delta);
	}
	else if (keystate[SDL_SCANCODE_UP])
	{
		fish->setY(y - static_cast<float>(FISH_VELOCITY*delta * sin(angle*M_PI/180)));
		fish->setX(x - static_cast<float>(FISH_VELOCITY*delta * cos(angle*M_PI/180)));
	}
	x = fish->getX();
	y = fish->getY();
	if (x <= 0) fish->setX(1);
	else if (x >= SCREEN_WIDTH - 51)fish->setX(SCREEN_WIDTH - 52);
	if (y <= 70) fish->setY(71);
	else if (y >= SCREEN_HEIGHT - 51) fish->setY(SCREEN_HEIGHT - 52);
}