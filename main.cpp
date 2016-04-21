#include "functions.h"

int main(int argc, char **argv)
{
	Uint32 t1 = SDL_GetTicks(), t2;
	int frames = 0;
	double delta, worldTime=0.0, fpsTimer=0.0, fps=0.0;

	Uint32 colours[12];
	char text[128];

	Game* game = new Game;
	//Fish* fish = new Fish("./images/ryba1.bmp");



	SDL_Event event;
	SDL_Surface *screen, *charset;
	SDL_Texture *scrtex;
	SDL_Window *window;
	SDL_Renderer *renderer;

	if(SDL_Init(SDL_INIT_TIMER|SDL_INIT_VIDEO) != 0)
	{
		return 1;
	}

	if (SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer) != 0)
	{
		SDL_Quit();
		return 1;
	}
	
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(window, "ARAP v0.05");

	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

	SDL_ShowCursor(SDL_DISABLE); // wy³¹czenie widocznoœci kursora myszy

	charset = SDL_LoadBMP("./images/cs8x8.bmp");
	if (charset == NULL)
	{
		FreeMemoryAndQuit(charset, screen, scrtex, window, renderer, game, NULL);
		return 1;
	}

	SDL_SetColorKey(charset, true, 0x000000);
	SetColours(screen->format, colours);
	srand(static_cast<unsigned int>(time(NULL)));
	Player **players = new Player*[4];
	for (int i = 0; i < 4; i++) {
		players[i] = new Player(i, i * 100, i * 150);
	}
	for (int i = 0; i < 4; i++)
		players[i]->fish->setText(renderer);
	while(!game->getStatus())
	{
		// w tym momencie t2-t1 to czas w milisekundach, jaki uplyna³ od ostatniego narysowania ekranu, delta to ten sam czas w sekundach
		t2 = SDL_GetTicks();
		delta = (t2 - t1) * 0.001;
		t1 = t2;

		SDL_FillRect(screen, NULL, colours[BLACK]);






		/*SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, paddle.texture[0]);
		SDL_RenderCopyEx(renderer, tex, &srcrect, &dstrect, (frames/1000)%360, NULL, SDL_FLIP_NONE);*/

		DrawRectangle(screen, 4, 4, SCREEN_WIDTH - 8, 50, colours[GREY3], colours[GREY2]);
		//DrawInfo(screen, charset, text, worldTime, fish, colours);

		fpsTimer += delta;
		if (fpsTimer > 0.5)
		{
			fps = frames * 2;
			frames = 0;
			fpsTimer -= 0.5;
		}

		sprintf(text, "%.0lf klatek/s", fps);
		DrawString(screen, 25, SCREEN_HEIGHT - 10, text, charset);

		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		for (int i = 0; i < 4; i++)
			DrawFish(renderer, players[i]->fish->getText(), (int)players[i]->fish->getX(), (int)players[i]->fish->getY(),players[i]->fish->angle); // rysuje paletkê
		SDL_RenderPresent(renderer);

		MoveFish(players[0]->fish, delta,players[0]->fish->angle); // ruch rybki

		SDL_PollEvent(&event); // obs³uga zdarzeñ
		if (event.type == SDL_QUIT)
		{
			game->setStatus(true);
		}
		frames++;
		t2 = SDL_GetTicks();
		delta = (t2 - t1) * 0.001;
		SDL_Delay(8.0 - delta);
	}
	// zwolnienie powierzchni
	FreeMemoryAndQuit(charset, screen, scrtex, window, renderer, game, NULL);
	return 0;
}
