#include "pong.h"

void init_display(struct Display *display) {
	SDL_Init(SDL_INIT_VIDEO);
	
	SDL_DisplayMode dm;
	SDL_GetDesktopDisplayMode(0, &dm);
	
	SDL_Window *window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, dm.w, dm.h, SDL_WINDOW_FULLSCREEN_DESKTOP);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	
	display->window = window;
	display->renderer = renderer;
	display->w = dm.w;
	display->h = dm.h;
}

void init_font(struct Display *display) {
	TTF_Init();
	
	display->font = TTF_OpenFont("FreeMono.ttf", 32);
}

void set_viewport(struct Display *display, int w, int h) {
	int goalW = (div(display->w, w)).quot;
	int goalH = (div(display->h, h)).quot;
	
	SDL_Rect goal;
	goal.x = 0;
	goal.y = 0;
	if (goalW < goalH) {
		goal.w = display->w / goalW;
		goal.h = display->h / goalW;
		SDL_RenderSetScale(display->renderer, goalW, goalW);
	} else {
		goal.w = display->w / goalH;
		goal.h = display->h / goalH;
		SDL_RenderSetScale(display->renderer, goalH, goalH);
	}
	
	display->xoffset = (goal.w - w)/2;
	display->yoffset = (goal.h - h)/2;
	
	FILE *log = fopen("log.txt", "wb");
	fprintf(log, "viewport: %d x %d\n", goal.w, goal.h);
	fprintf(log, "offset: (%d, %d)\n", display->xoffset, display->yoffset);
	fclose(log);

	if (SDL_RenderSetViewport(display->renderer, &goal) != 0) {
		exit(-1);
	}
}

int main(int argc, char *argv[]) {
	(void) argc;
	(void) argv;
	
	struct Display display;
	init_display(&display);
	init_font(&display);
	set_viewport(&display, 320, 240);
	
	/*
	FILE *log = fopen("log.txt", "wb");
	fprintf(log, "ratio: %f\n", 1.0f * display.w / display.h);
	fclose(log);
	*/
	
	bool running = true;
	SDL_Event event;
	SDL_Color white = {255, 255, 255, 0};
	SDL_Surface *text_surface = TTF_RenderText_Solid(display.font,"Pong",white);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(display.renderer, text_surface);
	SDL_Rect rect = {display.xoffset, display.yoffset, text_surface->w, text_surface->h};
	// SDL_Rect rect = {0, 0, text_surface->w, text_surface->h};
	SDL_FreeSurface(text_surface);
	SDL_Rect rect2 = {display.xoffset, display.yoffset, 320, 240};
	// SDL_Rect rect2 = {0, 0, 320, 240};
	SDL_Rect rect3 = {display.w - 10, display.h -10, 10, 10};
	while(running) {
		while (SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT: running = false; break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
						case SDLK_ESCAPE: running = false; break;
					}
				break;
			}
		}
		
		SDL_RenderClear(display.renderer);
		SDL_SetRenderDrawColor(display.renderer, 255, 0, 0, 255);
		SDL_RenderFillRect(display.renderer, &rect2);
		SDL_SetRenderDrawColor(display.renderer, 0, 255, 0, 255);
		SDL_RenderFillRect(display.renderer, &rect3);
		SDL_SetRenderDrawColor(display.renderer, 0, 0, 0, 255);
		SDL_RenderCopy(display.renderer, texture, NULL, &rect);
		SDL_RenderPresent(display.renderer);
	}
	
	TTF_CloseFont(display.font);
	TTF_Quit();
	SDL_DestroyRenderer(display.renderer);
	SDL_DestroyWindow(display.window);
	SDL_Quit();
	
	return 0;
}
