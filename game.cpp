#include "game.h"

Game::Game()
{
	this->quit = false;
	change = false;
	SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_SetWindowTitle(window, "ARAP v1.0");

	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_ShowCursor(SDL_DISABLE);
	charset = SDL_LoadBMP("./images/cs8x8.bmp");
	SDL_SetColorKey(charset, true, 0x000000);
	SetColours();

	isEvent = SDL_PollEvent(&event);
	srand(static_cast<unsigned int>(time(NULL)));

	players = new Player*[4];
	int pos[4][3] = { {50, 100, 180}, {700, 100, 0}, {50, 500, 180}, {700, 500, 0} };
	for (int i = 0; i < 4; i++)
	{
		players[i] = new Player(i, pos[i][0], pos[i][1], pos[i][2]);
		players[i]->getFish()->setTexture(renderer);
		collisions[i] = false;
	}
	server = NULL;
	client = NULL;

	delta = 0.016;
	frames = 0;
	worldTime = 0.0;
	fpsTimer = 0.0;
	fps = 0.0;
	MyNumber = 0;
	numberOfPlayers = 1;

	DrawMenu();

	Play();
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

// zwalnia powierzchnie i zamyka okno
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
	Fish* playerFish = player->getFish();

	float x = playerFish->getX();
	float y = playerFish->getY();
	int back = player->getBack();
	int predator = playerFish->getPredatorAngle();
	int angle = playerFish->getAngle();

	const Uint8* keystate = SDL_GetKeyboardState(NULL);

	if (back <= 0)
	{
		if (keystate[SDL_SCANCODE_RIGHT] && keystate[SDL_SCANCODE_LEFT]) {}
		else if (keystate[SDL_SCANCODE_UP] && keystate[SDL_SCANCODE_LEFT])
		{
			playerFish->setY(y - static_cast<float>(FISH_VELOCITY*delta * sin(angle*M_PI / 180)));
			playerFish->setX(x - static_cast<float>(FISH_VELOCITY*delta * cos(angle*M_PI / 180)));
			playerFish->setAngle(angle - int(200 * delta));
			change = true;
		}
		else if (keystate[SDL_SCANCODE_UP] && keystate[SDL_SCANCODE_RIGHT])
		{
			playerFish->setY(y - static_cast<float>(FISH_VELOCITY*delta * sin(angle*M_PI / 180)));
			playerFish->setX(x - static_cast<float>(FISH_VELOCITY*delta * cos(angle*M_PI / 180)));
			playerFish->setAngle(angle + int(200 * delta));
			change = true;
		}

		else if (keystate[SDL_SCANCODE_LEFT])
		{
			playerFish->setAngle(angle - int(200 * delta));
			change = true;
		}
		else if (keystate[SDL_SCANCODE_RIGHT])
		{
			playerFish->setAngle(angle + int(200 * delta));
			change = true;
		}
		else if (keystate[SDL_SCANCODE_UP])
		{
			playerFish->setY(y - static_cast<float>(FISH_VELOCITY * delta * sin(angle*M_PI / 180)));
			playerFish->setX(x - static_cast<float>(FISH_VELOCITY * delta * cos(angle*M_PI / 180)));
			change = true;
		}
	}
	else
	{
		//rybka odplyna w sina dal po zaatakowaniu przez predatora
		playerFish->setY(y - static_cast<float>(BACK_VELOCITY * delta * sin(predator*M_PI / 180)));
		playerFish->setX(x - static_cast<float>(BACK_VELOCITY * delta * cos(predator*M_PI / 180)));
		player->setBack(back - static_cast<int>(1000*delta));
		if (client != NULL)
		{
			Package* package = client->getPackage();
			package->back = back - static_cast<int>(1000 * delta);
		}
		change = true;
	}
	x = playerFish->getX();
	y = playerFish->getY();
	if (x <= 7) playerFish->setX(8);
	else if (x >= SCREEN_WIDTH - 56) playerFish->setX(SCREEN_WIDTH - 57);
	if (y <= 64) playerFish->setY(65);
	else if (y >= SCREEN_HEIGHT - 51) playerFish->setY(SCREEN_HEIGHT - 52);
	if (change)
	{
		Package* package = NULL;
		if (client != NULL)
		{
			package = client->getPackage();
		}
		else if (server != NULL)
		{
			package = server->getPackage();
		}
		if (package != NULL)
		{
			package->angle = playerFish->getAngle();
			package->number = MyNumber;
			package->x = (int)x;
			package->y = (int)y;
		}
	}
}

void Game::Collision()
{
	for (int i = 0; i < numberOfPlayers; i++) // i - atakujacy, j - ofiara
	{
		Fish* predatorFish = players[i]->getFish();
		float predatorPosX = predatorFish->getX() + 25.5f;
		float predatorPosY = predatorFish->getY() + 15.0f;
		int predatorAngle = predatorFish->getAngle();
		predatorPosX -= static_cast<float>(cos(predatorAngle*M_PI / 180) * 25.5f);
		predatorPosY -= static_cast<float>(sin(predatorAngle*M_PI / 180) * 25.5f);
		for (int j = 0; j < numberOfPlayers; j++)
		{
			if (i == j)
			{
				continue;
			}
			if (players[j]->getBack() <= 0)
			{
				Fish* victimFish = players[j]->getFish();
				int victimAngle = victimFish->getAngle();

				float victimPosX = victimFish->getX() + 25.5f;
				float victimPosY = victimFish->getY() + 15.0f;

				float upperLeftCornerX = victimPosX - 15.0f;;
				float upperLeftCornerY = victimPosY - 15.0f;

				float bottomRightCornerX = victimPosX + 15.0f;
				float bottomRightCornerY = victimPosY + 15.0f;

				if (predatorPosX >= upperLeftCornerX && predatorPosY >= upperLeftCornerY &&
					predatorPosX <= bottomRightCornerX && predatorPosY <= bottomRightCornerY)
				{
					players[i]->setPoints(players[i]->getPoints() + 10);
					players[j]->setBack(250);
					victimFish->setPredatorAngle(predatorAngle);
					Package* package = server->getPackage();
					package->points[i] = players[i]->getPoints();
					collisions[j] = true;
				}
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

// wyswietla informacje dotyczace gry
void Game::DrawInfo(char* text, Player** players)
{
	sprintf_s(text, 128, "Czas:   %.1lfs", worldTime);
	DrawString(12, 10, text);
	char name[5] = "YRBG";
	for (int i = 0; i < numberOfPlayers; i++)
	{
		sprintf_s(text, 128, "Gracz %c: %dpkt", name[i], players[i]->getPoints());
		DrawString(12+i*150, 26, text);
	}
}

// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y) - punkt srodka obrazka sprite na ekranie
void Game::DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y)
{
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
}

// rysowanie linii o dlugosci l w pionie (gdy horizontal = 0) lub w poziomie (gdy horizontal = 1)
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

// rysuje rybke
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
		if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && isEvent > 0 && event.key.keysym.sym == SDLK_ESCAPE)) // wcisniecie krzyzyka/ESC konczy apke
		{
			this->quit = true;
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
			{
				server = new Server();
			}
			else if (server->Accept(numberOfPlayers))
			{
				numberOfPlayers++;
			}
			DrawRectangle(SCREEN_WIDTH / 2 - 65, SCREEN_HEIGHT / 2 - 10, 130, 70, colours[GREY3], colours[GREY3]);
			DrawRectangle(SCREEN_WIDTH / 2 - 65, (SCREEN_HEIGHT / 2 + 29) + menuPosition * 10, 130, 10, colours[GREY], colours[GREY]);
			DrawString(SCREEN_WIDTH / 2 - 4*strlen(this->server->getIP()), SCREEN_HEIGHT / 2, this->server->getIP());
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
						server->SendPlayers(numberOfPlayers);
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
			if (client != NULL && client->isConnected() && client->ReceivePlayers())
			{
				numberOfPlayers = client->getIResult();
				break;
			}
			else if (client != NULL && client->isConnected())
			{
				DrawString(SCREEN_WIDTH / 2 - 40, SCREEN_HEIGHT / 2, "Start soon!");
				menuPosition = 2;
			}
			if (client == NULL || !client->isConnected())
			{
				DrawString(SCREEN_WIDTH / 2 - 40, SCREEN_HEIGHT / 2, "Podaj IP:");
				DrawString(SCREEN_WIDTH / 2 - 64, SCREEN_HEIGHT / 2 + 30, ip);
				DrawString(SCREEN_WIDTH / 2 - 16, SCREEN_HEIGHT / 2 + 40, "OK");
			}
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
						if (client == NULL)
						{
							client = new Client(ip);
						}
						else if(!client->isConnected() && client->Connect())
						{
							MyNumber = client->ReceiveNumber();
							client->setConnected(true);
						}
					}
					else if (menuPosition == 2)
					{
						menuIndex = 0;
						menuPosition = 1;
						client = NULL;
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
	while (!this->quit)
	{
		t1 = SDL_GetTicks();

		SDL_FillRect(this->screen, NULL, this->colours[BLACK]);

		DrawRectangle(4, 4, SCREEN_WIDTH - 8, 50, this->colours[GREY3], this->colours[GREY2]);
		DrawInfo(this->text, this->players);

		fpsTimer += delta;
		worldTime += delta;
		if (fpsTimer > 0.5)
		{
			fps = frames * 2;
			frames = 0;
			fpsTimer -= 0.5;
		}

		sprintf_s(text, 128, "%.0lf klatek/s", fps);
		DrawString(25, SCREEN_HEIGHT - 10, text);
		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);

		for (int i = 0; i < numberOfPlayers; i++)
		{
			DrawFish(players[i]->getFish()); // rysuje rybki
		}
		isEvent = SDL_PollEvent(&event);
		MoveFish(players[MyNumber]); // ruch rybki

		if (server != NULL) // kolizje
		{
			Collision();
			for (int i = 0; i < numberOfPlayers; i++)
			{
				int backTime = players[i]->getBack();
				if (backTime > 0)
				{
					players[i]->setBack(backTime - static_cast<int>(1000 * delta));
				}
			}
		}

		SDL_RenderPresent(renderer);

		if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && isEvent > 0 && event.key.keysym.sym == SDLK_ESCAPE)) // wcisniecie krzyzyka/ESC konczy apke
		{
			this->quit = true;
		}
		frames++;

		do
		{
			if (client != NULL)
			{
				if (change) 
				{
					client->Send();
					change = false;
				}
				if (client->Receive()) 
				{
					if (client->getIResult() == 0) return;
					Package* package = client->getPackage();
					players[package->number]->getFish()->setX(static_cast<float>(package->x));
					players[package->number]->getFish()->setY(static_cast<float>(package->y));
					players[package->number]->getFish()->setAngle(package->angle);
					players[0]->setPoints(package->points[0]);
					players[1]->setPoints(package->points[1]);
					players[2]->setPoints(package->points[2]);
					players[3]->setPoints(package->points[3]);
					players[MyNumber]->setBack(package->back);
					players[MyNumber]->getFish()->setPredatorAngle(package->predatorAngle);
				}
			}
			else if (server != NULL)
			{
				if (change)
				{
					for (int i = 1; i < numberOfPlayers; i++)
					{
						server->SendPosition(i);
					}
					change = false;
				}
				for (int i = 0; i < numberOfPlayers; i++)
				{
					if (collisions[i] == true)
					{
						Package* package = server->getPackage();
						package->back = players[i]->getBack();
						package->predatorAngle = players[i]->getFish()->getPredatorAngle();
						server->SendCollision(i);
						for (int j = 1; j < numberOfPlayers; j++)
						{
							server->SendScore(j);
						}
						collisions[i] = false;
					}
				}
				for (int i = 1; i < numberOfPlayers; i++)
				{
					if (server->Receive(i))
					{
						Package* package = server->getPackage();
						players[package->number]->getFish()->setX(static_cast<float>(package->x));
						players[package->number]->getFish()->setY(static_cast<float>(package->y));
						players[package->number]->getFish()->setAngle(package->angle);
						for (int j = 1; j < numberOfPlayers; j++)
						{
							if (j != package->number)
							{
								server->SendPosition(j);
							}
						}
					}
				}
			}
			t2 = SDL_GetTicks();
			delta = (t2 - t1) * 0.001;
		} while (delta < 0.016);
	}
	FreeMemoryAndQuit();
}