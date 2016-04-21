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