#include "Font.h"

int InitFont(){
    if (TTF_Init() == -1){
        return 1;
    }
    return 0;
}

void QuitFont(){
    TTF_Quit();
}
int LoadFont(const char*& path, const int& size, TTF_Font*& font){
    font = TTF_OpenFont(path, size);
    if (!font) return 1;
    return 0;
}

void RenderTextToTexture(SDL_Renderer*& renderer, SDL_Texture*& texture, TTF_Font*& font, const char*& text, SDL_Color& color){
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text, color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}
