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
#include "game.h"
#include "fish.h"
#include "player.h"


	/* other_functions.cpp */

void SetColours(SDL_PixelFormat*, Uint32*);

void FreeMemoryAndQuit(SDL_Surface*, SDL_Surface*, SDL_Texture*, SDL_Window*, SDL_Renderer*, Game*, Fish*);

void MoveFish(Fish*, double &,int&);


	/* draws_functions.cpp */

void DrawString(SDL_Surface*, int, int, const char*, SDL_Surface*);

void DrawInfo(SDL_Surface*, SDL_Surface*, char*, double &, Fish*, Uint32*);

void DrawSurface(SDL_Surface*, SDL_Surface*, int, int);

void DrawLine(SDL_Surface*, int, int, int, int, Uint32);

void DrawRectangle(SDL_Surface*, int, int, int, int, Uint32, Uint32);
void DrawFish(SDL_Renderer*, SDL_Texture*, int, int,int);