#include "Window.h"
int StartWindow(SDL_Window*& window_arg, const char*& nameofwindow, const int& w, const int& h){
	if (SDL_Init(SDL_INIT_VIDEO) != 0) return 1;

	window_arg = SDL_CreateWindow(nameofwindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);
	if (!window_arg) return 1;
	return 0;
}
void Close(SDL_Window*& window){SDL_DestroyWindow(window);}
int Renderer(SDL_Window*& window, SDL_Renderer*& renderer){
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) return 1;
	return 0;
}
void FreeRenderer(SDL_Renderer*& renderer){SDL_DestroyRenderer(renderer);}
