#include "Cursor.h"

bool PointInRect(const int& x, const int& y, const SDL_Rect& r) {
    return (x >= r.x && x <= r.x + r.w && y >= r.y && y <= r.y + r.h);
}

void ChangeCursor(SDL_Cursor*& cursor){
    SDL_SetCursor(cursor);
}