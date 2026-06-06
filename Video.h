#pragma once
extern "C"{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

#include <SDL2/SDL.h>

int LoadVideo(SDL_Renderer*& renderer, SDL_Texture*& texture, AVFormatContext*& formatCtx, AVCodecContext*& codecCtx, AVFrame*& frame, AVFrame*& yuvFrame, SwsContext*& swsCtx, int& videoStream, const char*& path, int* VideoW, int* VideoH);