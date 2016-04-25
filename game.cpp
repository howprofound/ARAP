#include "game.h"

Game::Game()
{
	this->quit = false;
	this->started = false;

	SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_SetWindowTitle(window, "ARAP v0.1");

	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_ShowCursor(SDL_DISABLE);
	charset = SDL_LoadBMP("./images/cs8x8.bmp");
	SDL_SetColorKey(charset, true, 0x000000);
	SetColours();

	isEvent = SDL_PollEvent(&event);
	srand(static_cast<unsigned int>(time(NULL)));

	players = new Player*[4];
	for (int i = 0; i < 4; i++)
	{
		players[i] = new Player(i, i * 100, i * 150);
	}

	for (int i = 0; i < 4; i++)
		players[i]->fish->setTexture(renderer);

	delta = 0.016;
	frames = 0;
	worldTime = 0.0;
	fpsTimer = 0.0;
	fps = 0.0;

	DrawMenu();

	Play();
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
void Game::SetColours()
{
	colours[BLACK] = SDL_MapRGB(screen->format, 0, 0, 0);
	colours[RED] = SDL_MapRGB(screen->format, 255, 16, 16);
	colours[ORANGE] = SDL_MapRGB(screen->format, 255, 150, 10);
	colours[YELLOW] = SDL_MapRGB(screen->format, 240, 240, 0);
	colours[GREEN] = SDL_MapRGB(screen->format, 0, 230, 0);
	colours[CYAN] = SDL_MapRGB(screen->format, 0, 235, 210);
	colours[BLUE] = SDL_MapRGB(screen->format, 15, 50, 210);
	colours[PINK] = SDL_MapRGB(screen->format, 255, 40, 210);
	colours[WHITE] = SDL_MapRGB(screen->format, 255, 255, 255);
	colours[GREY] = SDL_MapRGB(screen->format, 150, 150, 150);
	colours[GREY2] = SDL_MapRGB(screen->format, 100, 100, 100);
	colours[GREY3] = SDL_MapRGB(screen->format, 50, 50, 50);
}

// zwalnia powierzchniê i zamyka okno
void Game::FreeMemoryAndQuit()
{
	SDL_FreeSurface(charset);
	SDL_FreeSurface(screen);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

void Game::MoveFish(Fish* fish)
{
	float x = fish->getX();
	float y = fish->getY();
	int angle = fish->getAngle();

	const Uint8* keystate = SDL_GetKeyboardState(NULL);


	if (keystate[SDL_SCANCODE_RIGHT] && keystate[SDL_SCANCODE_LEFT]) {}
	else if (keystate[SDL_SCANCODE_UP] && keystate[SDL_SCANCODE_LEFT])
	{
		fish->setY(y - static_cast<float>(FISH_VELOCITY*delta * sin(angle*M_PI / 180)));
		fish->setX(x - static_cast<float>(FISH_VELOCITY*delta * cos(angle*M_PI / 180)));
		fish->setAngle(angle - int(200 * delta));
		//fish->angle -= int(200 * delta);
	}
	else if (keystate[SDL_SCANCODE_UP] && keystate[SDL_SCANCODE_RIGHT])
	{
		fish->setY(y - static_cast<float>(FISH_VELOCITY*delta * sin(angle*M_PI / 180)));
		fish->setX(x - static_cast<float>(FISH_VELOCITY*delta * cos(angle*M_PI / 180)));
		fish->setAngle(angle + int(200 * delta));
	}


	else if (keystate[SDL_SCANCODE_LEFT])
	{
		//fish->angle -= int(200 * delta);
		fish->setAngle(angle - int(200 * delta));
	}
	else if (keystate[SDL_SCANCODE_RIGHT])
	{
		//fish->angle += int(200 * delta);
		fish->setAngle(angle + int(200 * delta));
	}
	else if (keystate[SDL_SCANCODE_UP])
	{
		fish->setY(y - static_cast<float>(FISH_VELOCITY*delta * sin(angle*M_PI / 180)));
		fish->setX(x - static_cast<float>(FISH_VELOCITY*delta * cos(angle*M_PI / 180)));
	}
	x = fish->getX();
	y = fish->getY();
	if (x <= 0) fish->setX(1);
	else if (x >= SCREEN_WIDTH - 51)fish->setX(SCREEN_WIDTH - 52);
	if (y <= 70) fish->setY(71);
	else if (y >= SCREEN_HEIGHT - 51) fish->setY(SCREEN_HEIGHT - 52);
}
void Game::DrawString(int x, int y, const char* text)
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
void Game::DrawInfo(char* text, Player** players)
{
	sprintf(text, "Czas:   %.1lfs", worldTime);
	DrawString(12, 10, text);
	sprintf(text, "Gracz 1:   %dpkt", players[0]->getPoints());
	DrawString(12, 26, text);
	sprintf(text, "Gracz 2:   %dpkt", players[1]->getPoints());
	DrawString(162, 26, text);
	sprintf(text, "Gracz 3:   %dpkt", players[2]->getPoints());
	DrawString(312, 26, text);
	sprintf(text, "Gracz 4:   %dpkt", players[3]->getPoints());
	DrawString(462, 26, text);
}

// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y) - punkt œrodka obrazka sprite na ekranie
void Game::DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y)
{
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
}


// rysowanie linii o d³ugoœci l w pionie (gdy horizontal = 0), b¹dŸ w poziomie (gdy horizontal = 1)
void Game::DrawLine(int x, int y, int l, int horizontal, Uint32 color)
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
void Game::DrawRectangle(int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor)
{
	DrawLine(x, y, k, 0, outlineColor);
	DrawLine(x + l - 1, y, k, 0, outlineColor);
	DrawLine(x, y, l, 1, outlineColor);
	DrawLine(x, y + k - 1, l, 1, outlineColor);
	SDL_Rect rect = { x + 1, y + 1, l - 2, k - 2 };
	SDL_FillRect(screen, &rect, fillColor);
}

// rysuje paletkê
void Game::DrawFish(Fish *fish)
{
	SDL_Rect srcrect;
	SDL_Rect dstrect;

	srcrect.x = 0;
	srcrect.y = 0;
	srcrect.w = 51;
	srcrect.h = 30;
	dstrect.x = static_cast<int>(fish->getX());
	dstrect.y = static_cast<int>(fish->getY());
	dstrect.w = 51;
	dstrect.h = 30;
	int angle = fish->getAngle();
	SDL_RenderCopyEx(renderer, fish->getTexture(), &srcrect, &dstrect, angle, NULL, SDL_FLIP_NONE);
}

void Game::DrawMenu()
{
	int menuIndex = 0;
	int menuPosition = 0;
	int cursorPosition = 0;
	char ip[16] = "\0";
	while (true)
	{
		SDL_FillRect(this->screen, NULL, this->colours[BLACK]);

		isEvent = SDL_PollEvent(&event);
		if (event.type == SDL_QUIT) // wciœniêcie krzy¿yka koñczy apkê
		{
			this->setStatus(true);
			break;
		}

		if (menuIndex == 0)
		{
			DrawRectangle(SCREEN_WIDTH / 2 - 40, SCREEN_HEIGHT / 2 - 10, 80, 70, colours[GREY3], colours[GREY3]);
			DrawRectangle(SCREEN_WIDTH / 2 - 40, (SCREEN_HEIGHT / 2 + 29) + menuPosition * 10, 80, 10, colours[GREY], colours[GREY]);
			DrawString(SCREEN_WIDTH / 2 - 30, SCREEN_HEIGHT / 2, "Wybierz:");
			DrawString(SCREEN_WIDTH / 2 - 22, SCREEN_HEIGHT / 2 + 30, "Serwer");
			DrawString(SCREEN_WIDTH / 2 - 22, SCREEN_HEIGHT / 2 + 40, "Klient");
			if (event.type == SDL_KEYDOWN && isEvent > 0)
			{
				if (event.key.keysym.sym == SDLK_DOWN) // w dó³
				{
					menuPosition++;
					menuPosition %= 2;
				}
				else if (event.key.keysym.sym == SDLK_UP) // w górê
				{
					menuPosition--;
					if (menuPosition < 0)
					{
						menuPosition = 1;
					}
				}
				else if (event.key.keysym.sym == SDLK_RETURN) // enter
				{
					if (menuPosition == 0)
					{
						menuIndex = 1;
					}
					else
					{
						menuIndex = 2;
						menuPosition = 0;
					}
				}
			}
		}
		else if (menuIndex == 1)
		{
			DrawRectangle(SCREEN_WIDTH / 2 - 55, SCREEN_HEIGHT / 2 - 10, 110, 70, colours[GREY3], colours[GREY3]);
			DrawRectangle(SCREEN_WIDTH / 2 - 55, (SCREEN_HEIGHT / 2 + 29) + menuPosition * 10, 110, 10, colours[GREY], colours[GREY]);
			DrawString(SCREEN_WIDTH / 2 - 30, SCREEN_HEIGHT / 2, "IP: xxx");
			DrawString(SCREEN_WIDTH / 2 - 46, SCREEN_HEIGHT / 2 + 30, "Zacznij gre!");
			DrawString(SCREEN_WIDTH / 2 - 14, SCREEN_HEIGHT / 2 + 40, "Wroc");
			if (event.type == SDL_KEYDOWN && isEvent > 0)
			{
				if (event.key.keysym.sym == SDLK_DOWN) // w dó³
				{
					menuPosition++;
					menuPosition %= 2;
				}
				else if (event.key.keysym.sym == SDLK_UP) // w górê
				{
					menuPosition--;
					if (menuPosition < 0)
					{
						menuPosition = 1;
					}
				}
				else if (event.key.keysym.sym == SDLK_RETURN) // enter
				{
					if (menuPosition == 0)
					{
						break;
					}
					else
					{
						menuIndex = 0;
						menuPosition = 0;
					}
				}
			}
		}
		else if (menuIndex == 2)
		{
			DrawRectangle(SCREEN_WIDTH / 2 - 75, SCREEN_HEIGHT / 2 - 10, 140, 80, colours[GREY3], colours[GREY3]);
			DrawRectangle(SCREEN_WIDTH / 2 - 75, (SCREEN_HEIGHT / 2 + 29) + menuPosition * 10, 140, 10, colours[GREY], colours[GREY]);
			DrawString(SCREEN_WIDTH / 2 - 40, SCREEN_HEIGHT / 2, "Podaj IP:");
			DrawString(SCREEN_WIDTH / 2 - 64, SCREEN_HEIGHT / 2 + 30, ip);
			DrawString(SCREEN_WIDTH / 2 - 16, SCREEN_HEIGHT / 2 + 40, "OK");
			DrawString(SCREEN_WIDTH / 2 - 24, SCREEN_HEIGHT / 2 + 50, "Wroc");
			if (event.type == SDL_KEYDOWN && isEvent > 0)
			{
				if (menuPosition == 0)
				{
					if (event.key.keysym.sym >= SDLK_0 && event.key.keysym.sym <= SDLK_9)
					{
						if (cursorPosition < 15)
						{
							ip[cursorPosition] = event.key.keysym.sym;
							cursorPosition++;
							ip[cursorPosition] = '\0';
						}
					}
					else if (event.key.keysym.sym == SDLK_PERIOD)
					{
						if (cursorPosition < 15)
						{
							ip[cursorPosition] = '.';
							cursorPosition++;
							ip[cursorPosition] = '\0';
						}
					}
					else if (event.key.keysym.sym == SDLK_BACKSPACE)
					{
						if (cursorPosition > 0)
						{
							cursorPosition--;
							ip[cursorPosition] = '\0';
						}
					}
				}
				if (event.key.keysym.sym == SDLK_DOWN) // w dó³
				{
					menuPosition++;
					menuPosition %= 3;
				}
				else if (event.key.keysym.sym == SDLK_UP) // w górê
				{
					menuPosition--;
					if (menuPosition < 0)
					{
						menuPosition = 2;
					}
				}
				else if (event.key.keysym.sym == SDLK_RETURN) // enter
				{
					if (menuPosition == 1)
					{
						break;
					}
					else if (menuPosition == 2)
					{
						menuIndex = 0;
						menuPosition = 1;
					}
				}
			}
		}
		SDL_UpdateTexture(this->scrtex, NULL, this->screen->pixels, this->screen->pitch);
		SDL_RenderCopy(this->renderer, this->scrtex, NULL, NULL);

		SDL_RenderPresent(this->renderer);
	}
}

void Game::Play()
{
	while (!this->getStatus())
	{
		t1 = SDL_GetTicks();

		SDL_FillRect(this->screen, NULL, this->colours[BLACK]);

		DrawRectangle(4, 4, SCREEN_WIDTH - 8, 50, this->colours[GREY3], this->colours[GREY2]);
		DrawInfo(this->text, this->players);

		fpsTimer += delta;
		if (fpsTimer > 0.5)
		{
			fps = frames * 2;
			frames = 0;
			fpsTimer -= 0.5;
		}

		sprintf(text, "%.0lf klatek/s", fps);
		DrawString(25, SCREEN_HEIGHT - 10, text);
		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);


		for (int i = 0; i < 4; i++)
		{
			DrawFish(players[i]->fish); // rysuje rybkê
		}
		isEvent = SDL_PollEvent(&event);
		MoveFish(players[0]->fish); // ruch rybki

		SDL_RenderPresent(renderer);

		if (event.type == SDL_QUIT)
		{
			this->setStatus(true);
		}
		frames++;

		do
		{
			t2 = SDL_GetTicks();
			delta = (t2 - t1) * 0.001;
		} while (delta < 0.016);
	}
	FreeMemoryAndQuit();
}