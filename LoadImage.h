#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

int InitImage();
void QuitImage();

int LoadTexture(SDL_Texture*& texture, SDL_Renderer*& renderer, const char*& filename);