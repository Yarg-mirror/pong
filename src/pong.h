#ifndef HAVE_PONG_H
#define HAVE_PONG_H

#include <stdbool.h>
#include <SDL2/SDL.h>
struct Display {
	SDL_Window *window;
	SDL_Renderer *renderer;
	int w;
	int h;
	int xoffset;
	int yoffset;
};


#endif /* HAVE_PONG_H */
