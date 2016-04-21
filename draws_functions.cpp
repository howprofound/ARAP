#include "functions.h"

// narysowanie napisu txt na powierzchni screen, zaczynaj¹c od punktu (x, y) - charset to bitmapa 128x128 zawieraj¹ca znaki
void DrawString(SDL_Surface* screen, int x, int y, const char* text, SDL_Surface* charset)
{
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while (*text)
	{
		c = *text & 0xFF;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
	}
}

// wyœwietla informacje dotycz¹ce gry
void DrawInfo(SDL_Surface* screen, SDL_Surface* charset, char* text, double &worldTime, Fish* fish, Uint32* colours)
{
	sprintf(text, "Czas:   %.1lfs", worldTime);
	DrawString(screen, 12, 10, text, charset);
//	sprintf(text, "Punkty: %d", fish->getPoints());
	DrawString(screen, 12, 26, text, charset);

}

// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y) - punkt œrodka obrazka sprite na ekranie
void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y)
{
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
}


// rysowanie linii o d³ugoœci l w pionie (gdy horizontal = 0), b¹dŸ w poziomie (gdy horizontal = 1)
void DrawLine(SDL_Surface* screen, int x, int y, int l, int horizontal, Uint32 color)
{
	SDL_Rect rect;
	if (horizontal == 1)
	{
		rect.x = x;
		rect.y = y;
		rect.w = l;
		rect.h = 1;
	}
	else
	{
		rect.x = x;
		rect.y = y;
		rect.w = 1;
		rect.h = l;
	}
	SDL_FillRect(screen, &rect, color);
}

// rysowanie prostok¹ta o d³ugoœci boków l i k
void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor)
{
	DrawLine(screen, x, y, k, 0, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, outlineColor);
	DrawLine(screen, x, y, l, 1, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, outlineColor);
	SDL_Rect rect = { x + 1, y + 1, l - 2, k - 2 };
	SDL_FillRect(screen, &rect, fillColor);
}

// rysuje paletkê
void DrawFish(SDL_Renderer* renderer, SDL_Texture* fishTexture, int x, int y,int angle)
{
	SDL_Rect srcrect;
	SDL_Rect dstrect;

	srcrect.x = 0;
	srcrect.y = 0;
	srcrect.w = 51;
	srcrect.h = 30;
	dstrect.x = x;
	dstrect.y = y;
	dstrect.w = 51;
	dstrect.h = 30;
	SDL_RenderCopyEx(renderer, fishTexture, &srcrect, &dstrect, angle, NULL, SDL_FLIP_NONE);
}

void DrawMenu(SDL_Surface* screen, SDL_Surface* charset, SDL_Event* event, Uint32* colours, Player** players, int isEvent, Game* game)
{
	int menuIndex = players[0]->getMenuIndex();
	int menuPosition = players[0]->getMenuPosition();
	if (menuIndex == 0)
	{
		DrawRectangle(screen, SCREEN_WIDTH / 2 - 40, SCREEN_HEIGHT / 2 - 10, 80, 70, colours[GREY3], colours[GREY3]);
		DrawRectangle(screen, SCREEN_WIDTH / 2 - 40, (SCREEN_HEIGHT / 2 + 29)+menuPosition*10, 80, 10, colours[GREY], colours[GREY]);
		DrawString(screen, SCREEN_WIDTH / 2 - 30, SCREEN_HEIGHT / 2, "Wybierz:", charset);
		DrawString(screen, SCREEN_WIDTH / 2 - 22, SCREEN_HEIGHT / 2 + 30, "Serwer", charset);
		DrawString(screen, SCREEN_WIDTH / 2 - 22, SCREEN_HEIGHT / 2 + 40, "Klient", charset);
		if (event->type == SDL_KEYDOWN && isEvent > 0)
		{
			if (event->key.keysym.sym == SDLK_DOWN) // w dó³
			{
				menuPosition++;
				menuPosition %= 2;
				players[0]->setMenuPosition(menuPosition);
			}
			else if (event->key.keysym.sym == SDLK_UP) // w górê
			{
				menuPosition--;
				if (menuPosition < 0)
				{
					menuPosition = 1;
				}
				players[0]->setMenuPosition(menuPosition);
			}
			else if (event->key.keysym.sym == SDLK_RETURN) // enter
			{
				if (menuPosition == 0)
				{
					players[0]->setMenuIndex(1);
				}
				else
				{
					players[0]->setMenuIndex(2);
					players[0]->setMenuPosition(0);
				}
			}
		}
	}
	else if (menuIndex == 1)
	{
		DrawRectangle(screen, SCREEN_WIDTH / 2 - 55, SCREEN_HEIGHT / 2 - 10, 110, 70, colours[GREY3], colours[GREY3]);
		DrawRectangle(screen, SCREEN_WIDTH / 2 - 55, (SCREEN_HEIGHT / 2 + 29) + menuPosition * 10, 110, 10, colours[GREY], colours[GREY]);
		DrawString(screen, SCREEN_WIDTH / 2 - 30, SCREEN_HEIGHT / 2, "IP: xxx", charset);
		DrawString(screen, SCREEN_WIDTH / 2 - 46, SCREEN_HEIGHT / 2 + 30, "Zacznij gre!", charset);
		DrawString(screen, SCREEN_WIDTH / 2 - 14, SCREEN_HEIGHT / 2 + 40, "Wroc", charset);
		if (event->type == SDL_KEYDOWN && isEvent > 0)
		{
			if (event->key.keysym.sym == SDLK_DOWN) // w dó³
			{
				menuPosition++;
				menuPosition %= 2;
				players[0]->setMenuPosition(menuPosition);
			}
			else if (event->key.keysym.sym == SDLK_UP) // w górê
			{
				menuPosition--;
				if (menuPosition < 0)
				{
					menuPosition = 1;
				}
				players[0]->setMenuPosition(menuPosition);
			}
			else if (event->key.keysym.sym == SDLK_RETURN) // enter
			{
				if (menuPosition == 0)
				{
					game->setIsStarted(true);
				}
				else
				{
					players[0]->setMenuIndex(0);
				}
			}
		}
	}
	else if (menuIndex == 2)
	{
		DrawRectangle(screen, SCREEN_WIDTH / 2 - 45, SCREEN_HEIGHT / 2 - 10, 90, 70, colours[GREY3], colours[GREY3]);
		DrawRectangle(screen, SCREEN_WIDTH / 2 - 45, (SCREEN_HEIGHT / 2 + 29) + menuPosition * 10, 90, 10, colours[GREY], colours[GREY]);
		DrawString(screen, SCREEN_WIDTH / 2 - 35, SCREEN_HEIGHT / 2, "Podaj IP:", charset);
		DrawString(screen, SCREEN_WIDTH / 2 - 11, SCREEN_HEIGHT / 2 + 30, "OK", charset);
		DrawString(screen, SCREEN_WIDTH / 2 - 19, SCREEN_HEIGHT / 2 + 40, "Wroc", charset);
		if (event->type == SDL_KEYDOWN && isEvent > 0)
		{
			if (event->key.keysym.sym == SDLK_DOWN) // w dó³
			{
				menuPosition++;
				menuPosition %= 2;
				players[0]->setMenuPosition(menuPosition);
			}
			else if (event->key.keysym.sym == SDLK_UP) // w górê
			{
				menuPosition--;
				if (menuPosition < 0)
				{
					menuPosition = 1;
				}
				players[0]->setMenuPosition(menuPosition);
			}
			else if (event->key.keysym.sym == SDLK_RETURN) // enter
			{
				if (menuPosition == 0)
				{
					game->setIsStarted(true);
				}
				else
				{
					players[0]->setMenuIndex(0);
				}
			}
		}
	}
}