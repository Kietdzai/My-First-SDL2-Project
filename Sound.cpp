#include "Sound.h"

int FirstStartAudio(){
    if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0) return 1;
    return 0;
}
int InitSound(){return Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);}
int LoadChunk(Mix_Chunk*& sound, const char* path){
    sound = Mix_LoadWAV(path);
    if (!sound) return 1;
    return 0;
}
int LoadMusic(Mix_Music*& music, const char* path){
    music = Mix_LoadMUS(path);
    if (!music) return 1;
    return 0;
}
void SoundCleanup(){
    Mix_CloseAudio();
    Mix_Quit();
}