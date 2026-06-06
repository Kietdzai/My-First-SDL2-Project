#pragma once
#include <SDL2/SDL.h>
#include "LoadImage.h"
#include "Font.h"
#include "Questions.h"
#include "Answer.h"

int StartWindow(SDL_Window*& window_arg, const char*& nameofwindow, const int& w, const int& h);
void Close(SDL_Window*& window);
int Renderer(SDL_Window*& window, SDL_Renderer*& renderer_arg);
void FreeRenderer(SDL_Renderer*& renderer);
