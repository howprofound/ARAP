#include "game.h"

Game::Game()
{
	this->quit = false;
	this->started = false;
	change = false;
	SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_SetWindowTitle(window, "ARAP v0.2");

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
		players[i] = new Player(i, (i+1) * 100, (i+1) * 150);
	}
	for (int i = 0; i < 4; i++)
		players[i]->fish->setTexture(renderer);
	server = NULL;
	client = NULL;

	delta = 0.016;
	frames = 0;
	worldTime = 0.0;
	fpsTimer = 0.0;
	fps = 0.0;
	MyNumber = 0;
	NumberOfPlayers = 1;

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

// zwalnia powierzchni� i zamyka okno
void Game::FreeMemoryAndQuit()
{
	SDL_FreeSurface(charset);
	SDL_FreeSurface(screen);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

void Game::MoveFish(Player* player)
{
	//float x = fish->getX();
	//float y = fish->getY();
	//int angle = fish->getAngle();

	float x = player->fish->getX();
	float y = player->fish->getY();
	float back = player->fish->getBack();
	float predator = player->fish->getPredator();
	int angle = player->fish->getAngle();

	const Uint8* keystate = SDL_GetKeyboardState(NULL);

	if (back <= 0)
	{
		if (keystate[SDL_SCANCODE_RIGHT] && keystate[SDL_SCANCODE_LEFT]) {}
		else if (keystate[SDL_SCANCODE_UP] && keystate[SDL_SCANCODE_LEFT])
		{
			player->fish->setY(y - static_cast<float>(FISH_VELOCITY*delta * sin(angle*M_PI / 180)));
			player->fish->setX(x - static_cast<float>(FISH_VELOCITY*delta * cos(angle*M_PI / 180)));
			player->fish->setAngle(angle - (200 * delta));
			change = true;
			//fish->angle -= int(200 * delta);
		}
		else if (keystate[SDL_SCANCODE_UP] && keystate[SDL_SCANCODE_RIGHT])
		{
			player->fish->setY(y - static_cast<float>(FISH_VELOCITY*delta * sin(angle*M_PI / 180)));
			player->fish->setX(x - static_cast<float>(FISH_VELOCITY*delta * cos(angle*M_PI / 180)));
			player->fish->setAngle(angle + int(200 * delta));
			change = true;
		}

		else if (keystate[SDL_SCANCODE_LEFT])
		{
			//fish->angle -= int(200 * delta);
			player->fish->setAngle(angle - int(200 * delta));
			change = true;
		}
		else if (keystate[SDL_SCANCODE_RIGHT])
		{
			//fish->angle += int(200 * delta);
			player->fish->setAngle(angle + int(200 * delta));
			change = true;
		}
		else if (keystate[SDL_SCANCODE_UP])
		{
			player->fish->setY(y - static_cast<float>(FISH_VELOCITY*delta * sin(angle*M_PI / 180)));
			player->fish->setX(x - static_cast<float>(FISH_VELOCITY*delta * cos(angle*M_PI / 180)));
			change = true;

		}
		if (change) {
			if (client != NULL) {
				client->package.angle = player->fish->getAngle();
				client->package.number = MyNumber;
				client->package.x = (int)player->fish->getX();
				client->package.y = (int)player->fish->getY();
			}
			else if (server != NULL) {
				server->package.angle = player->fish->getAngle();
				server->package.number = MyNumber;
				server->package.x = (int)player->fish->getX();
				server->package.y = (int)player->fish->getY();
			}
		}
		
	}
	else
	{
		//rybka odplyna w sina dal po zaatakowaniu przez predatora
		player->fish->setY(y - static_cast<float>(100 * delta * sin(predator*M_PI / 180)));
		player->fish->setX(x - static_cast<float>(100 * delta * cos(predator*M_PI / 180)));
		player->fish->setBack(back - delta);
		/*x = player->fish->getX();
		y = player->fish->getY();
		if (x <= 0) player->fish->setX(1);
		else if (x >= SCREEN_WIDTH - 51)player->fish->setX(SCREEN_WIDTH - 52);
		if (y <= 64) player->fish->setY(65);
		else if (y >= SCREEN_HEIGHT - 51) player->fish->setY(SCREEN_HEIGHT - 52);*/
	}
	x = player->fish->getX();
	y = player->fish->getY();
	if (x <= 7) player->fish->setX(8);
	else if (x >= SCREEN_WIDTH - 56) player->fish->setX(SCREEN_WIDTH - 57);
	if (y <= 64) player->fish->setY(65);
	else if (y >= SCREEN_HEIGHT - 51) player->fish->setY(SCREEN_HEIGHT - 52);
}

void Game::Collision(Player *player)
{
	float x = player->fish->getX() + 25.5;
	float y = player->fish->getY() - 15;
	int angle = player->fish->getAngle();
	y -= sin(angle*M_PI / 180) * 15;
	x -= cos(angle*M_PI / 180) * 15;
	for (int i = 0; i < 4; i++)
	{
		if (player->getNumber() == i)
			continue;
		else
		{
			float xTemp = players[i]->fish->getX() + 25.5;
			float yTemp = players[i]->fish->getY() - 15;
			if ((x <= (xTemp + 15) && y <= (yTemp + 15)) && (x >= (xTemp - 15) && y >= (yTemp - 15)) &&
				(x >= (xTemp - 15) && y <= (yTemp + 15)) && (x <= (xTemp + 15) && y >= (yTemp - 15)))
			{
				player->setPoints(player->getPoints() + 1);
				players[i]->fish->setBack(0.5);
				players[i]->fish->setPredator(player->fish->getAngle());
			}
		}
	}
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

// wy�wietla informacje dotycz�ce gry
void Game::DrawInfo(char* text, Player** players)
{
	sprintf(text, "Czas:   %.1lfs", worldTime);
	DrawString(12, 10, text);
	sprintf(text, "Gracz Y:   %dpkt", players[0]->getPoints());
	DrawString(12, 26, text);
	sprintf(text, "Gracz R:   %dpkt", players[1]->getPoints());
	DrawString(162, 26, text);
	sprintf(text, "Gracz B:   %dpkt", players[2]->getPoints());
	DrawString(312, 26, text);
	sprintf(text, "Gracz G:   %dpkt", players[3]->getPoints());
	DrawString(462, 26, text);
}

// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y) - punkt �rodka obrazka sprite na ekranie
void Game::DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y)
{
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
}


// rysowanie linii o d�ugo�ci l w pionie (gdy horizontal = 0), b�d� w poziomie (gdy horizontal = 1)
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

// rysowanie prostokata o dlugosci bokow l i k
void Game::DrawRectangle(int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor)
{
	DrawLine(x, y, k, 0, outlineColor);
	DrawLine(x + l - 1, y, k, 0, outlineColor);
	DrawLine(x, y, l, 1, outlineColor);
	DrawLine(x, y + k - 1, l, 1, outlineColor);
	SDL_Rect rect = { x + 1, y + 1, l - 2, k - 2 };
	SDL_FillRect(screen, &rect, fillColor);
}

// rysuje paletke
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
		if (event.type == SDL_QUIT) // wcisniecie krzyzyka konczy apke
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
				if (event.key.keysym.sym == SDLK_DOWN) // w dol
				{
					menuPosition++;
					menuPosition %= 2;
				}
				else if (event.key.keysym.sym == SDLK_UP) // w gore
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
			if (server == NULL)
				server = new Server();
			else if (server->Accept(NumberOfPlayers))
				NumberOfPlayers++;
			DrawRectangle(SCREEN_WIDTH / 2 - 55, SCREEN_HEIGHT / 2 - 10, 110, 70, colours[GREY3], colours[GREY3]);
			DrawRectangle(SCREEN_WIDTH / 2 - 55, (SCREEN_HEIGHT / 2 + 29) + menuPosition * 10, 110, 10, colours[GREY], colours[GREY]);
			DrawString(SCREEN_WIDTH / 2 - 30, SCREEN_HEIGHT / 2, this->server->IP);
			DrawString(SCREEN_WIDTH / 2 - 46, SCREEN_HEIGHT / 2 + 30, "Zacznij gre!");
			DrawString(SCREEN_WIDTH / 2 - 14, SCREEN_HEIGHT / 2 + 40, "Wroc");
			if (event.type == SDL_KEYDOWN && isEvent > 0)
			{
				if (event.key.keysym.sym == SDLK_DOWN) // w dol
				{
					menuPosition++;
					menuPosition %= 2;
				}
				else if (event.key.keysym.sym == SDLK_UP) // w gore
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
						server = NULL;
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
				if (event.key.keysym.sym == SDLK_DOWN) // w dol
				{
					menuPosition++;
					menuPosition %= 3;
				}
				else if (event.key.keysym.sym == SDLK_UP) // w gore
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
						if (client == NULL) {
							client = new Client(ip);
						}
						else if(client->Connect()){
							MyNumber = client->RecieveNumber();
							break;
						}
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


		for (int i = 0; i < 2; i++)
		{
			DrawFish(players[i]->fish); // rysuje rybke
		}
		isEvent = SDL_PollEvent(&event);
		MoveFish(players[MyNumber]); // ruch rybki
		Collision(players[MyNumber]);

		SDL_RenderPresent(renderer);

		if (event.type == SDL_QUIT)
		{
			this->setStatus(true);
		}
		frames++;

		do
		{
			if (client != NULL) {
				if (change) {
					client->Send();
					change = false;
				}
				if (client->R()) {
					players[client->package.number]->fish->setX(client->package.x);
					players[client->package.number]->fish->setY(client->package.y);
					players[client->package.number]->fish->setAngle(client->package.angle);
				}
			}
			else if (server != NULL) {
				if (change) {
					server->S();
					change = false;
				}
				if (server->R()) {
					players[server->package.number]->fish->setX(server->package.x);
					players[server->package.number]->fish->setY(server->package.y);
					players[server->package.number]->fish->setAngle(server->package.angle);
				}
			}
			t2 = SDL_GetTicks();
			delta = (t2 - t1) * 0.001;
		} while (delta < 0.016);
	}
	FreeMemoryAndQuit();
}