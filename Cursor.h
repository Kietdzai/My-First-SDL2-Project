#pragma once
#include <SDL2/SDL.h>

bool PointInRect(const int& x, const int& y, const SDL_Rect& r);
void ChangeCursor(SDL_Cursor*& cursor);