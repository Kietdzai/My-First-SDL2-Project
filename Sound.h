#pragma once
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL.h>

int FirstStartAudio();
int InitSound();
int LoadChunk(Mix_Chunk*& sound, const char* path);
int LoadMusic(Mix_Music*& music, const char* path);
void SoundCleanup();