#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Button.h"
#include "Font.h"
#include <string>

void Update_CreateTexture(SDL_Renderer* renderer, Counter& ui, TTF_Font*& font);