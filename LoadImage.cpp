#include "LoadImage.h"

int InitImage(){
    if (IMG_Init(IMG_INIT_PNG) != 2) return 1;
    return 0;
}
void QuitImage(){IMG_Quit();}

// Hàm này load Texture và thay đổi trực tiếp SDL_Texture* được truyền vào
int LoadTexture(SDL_Texture*& texture, SDL_Renderer*& renderer, const char*& filename){
    SDL_Surface* LoadedSurface = IMG_Load(filename);
    if (!LoadedSurface) return 1;
    texture = SDL_CreateTextureFromSurface(renderer, LoadedSurface);
    if (!texture) {SDL_FreeSurface(LoadedSurface); return 1;}
    SDL_FreeSurface(LoadedSurface); // Giải phóng bộ nhớ
    return 0;
}