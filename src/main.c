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

void set_viewport(struct Display *display, int w, int h) {
	int goalW = (div(display->w, w)).quot;
	int goalH = (div(display->h, h)).quot;
	
	SDL_Rect goal;
	goal.x = 0;
	goal.y = 0;
	if (goalW < goalH) {
		goal.w = display->w / goalW;
		goal.h = display->h / goalW;
	} else {
		goal.w = display->w / goalH;
		goal.h = display->h / goalH;
	}
	
	display->xoffset = (goal.w - w)/2;
	display->yoffset = (goal.h - h)/2;
	
	SDL_RenderSetViewport(display->renderer, &goal);
	FILE *log = fopen("log.txt", "wb");
	fprintf(log, "viewport: %d x %d\n", goal.w, goal.h);
	fprintf(log, "offset: (%d, %d)\n", display->xoffset, display->yoffset);
	fclose(log);
}

int main(int argc, char *argv[]) {
	(void) argc;
	(void) argv;
	
	struct Display display;
	init_display(&display);
	set_viewport(&display, 320, 240);
	
	/*
	FILE *log = fopen("log.txt", "wb");
	fprintf(log, "ratio: %f\n", 1.0f * display.w / display.h);
	fclose(log);
	*/
	
	bool running = true;
	SDL_Event event;
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
		SDL_RenderPresent(display.renderer);
	}
	
	SDL_DestroyRenderer(display.renderer);
	SDL_DestroyWindow(display.window);
	SDL_Quit();
	
	return 0;
}
