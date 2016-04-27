#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS

#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	600

#define FISH_VELOCITY 400.0

#define BLACK 0
#define RED 1
#define ORANGE 2
#define YELLOW 3
#define GREEN 4
#define CYAN 5
#define BLUE 6
#define PINK 7
#define WHITE 8
#define GREY 9
#define GREY2 10
#define GREY3 11

#include <math.h>
#include <ctime>
#include <iostream>
#include "./sdl/include/SDL.h"
#include "./sdl/include/SDL_main.h"
#include "player.h"
#include "server.h"
#include "client.h"

class Game
{
private:
	bool started;
	bool quit;
	Uint32 t1, t2;
	int frames;
	int isEvent;
	double delta, worldTime, fpsTimer, fps;
	Uint32 colours[12];
	char text[128];
	SDL_Event event;
	SDL_Surface *screen, *charset;
	SDL_Texture *scrtex;
	SDL_Window *window;
	SDL_Renderer *renderer;
	Player **players;
	Server *server;
	Client *client;
	int MyNumber;
	int NumberOfPlayers;
	bool change;


public:
	Game();
private:
	void setStatus(bool);
	bool getStatus();
	void setIsStarted(bool);
	bool isStarted();


	void Play();
	void SetColours();

	void FreeMemoryAndQuit();

	void MoveFish(Player*);
	void Collision(Player*);
	/*------------------------------------------------------------DRAW------------------------------------------------------------*/
	void DrawString(int, int, const char*);

	void DrawInfo(char*, Player**);

	void DrawSurface(SDL_Surface*, SDL_Surface*, int, int);

	void DrawLine( int, int, int, int, Uint32);

	void DrawRectangle(int, int, int, int, Uint32, Uint32);

	void DrawFish(Fish *fish);

	void DrawMenu();
};