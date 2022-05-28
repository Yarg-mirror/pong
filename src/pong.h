#ifndef HAVE_PONG_H
#define HAVE_PONG_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
struct Display {
	SDL_Window *window;
	SDL_Renderer *renderer;
	int w;
	int h;
	int xoffset;
	int yoffset;
	TTF_Font *font;
};


#endif /* HAVE_PONG_H */
