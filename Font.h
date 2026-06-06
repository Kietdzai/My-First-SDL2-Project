#pragma once
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>

int InitFont();
void QuitFont();

int LoadFont(const char*& path, const int& size, TTF_Font*& font);

void RenderTextToTexture(SDL_Renderer*& renderer, SDL_Texture*& texture, TTF_Font*& font, const char*& text, SDL_Color& color);