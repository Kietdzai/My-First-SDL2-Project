#include "Video.h"
// hàm này chạy tất cả những gì cần làm trước khi render video

int LoadVideo(SDL_Renderer*& renderer, SDL_Texture*& texture, AVFormatContext*& formatCtx, AVCodecContext*& codecCtx, AVFrame*& frame, AVFrame*& yuvFrame, SwsContext*& swsCtx, int& videoStream, const char*& path, int* VideoW, int* VideoH){
    videoStream = -1;
    int return_code;

    return_code = avformat_open_input(&formatCtx, path, nullptr, nullptr);

    if(return_code < 0)
    {
        return -1;
    }

    return_code = avformat_find_stream_info(formatCtx, nullptr);

    if(return_code < 0) {
        return -1;
    }

    for(unsigned int i = 0; i < formatCtx->nb_streams; i++){
        if(formatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO){
            videoStream = i;
            break;
        }
    }

    if(videoStream == -1){
        return -1;
    }

    AVCodecParameters* codecpar = formatCtx->streams[videoStream]->codecpar;

    const AVCodec* codec = avcodec_find_decoder(codecpar->codec_id);

    if(codec == nullptr){
        return -1;
    }

    codecCtx = avcodec_alloc_context3(codec);

    if(codecCtx == nullptr) {
        return -1;
    }

    return_code = avcodec_parameters_to_context(codecCtx, codecpar);

    if(return_code < 0) {
        return -1;
    }

    return_code = avcodec_open2(codecCtx, codec, nullptr);

    if(return_code < 0) {
        return -1;
    }

    *VideoW = codecCtx->width;
    *VideoH = codecCtx->height;

    frame = av_frame_alloc();
    yuvFrame = av_frame_alloc();

    if(frame == nullptr || yuvFrame == nullptr) {
        return -1;
    }

    swsCtx = sws_getContext(*VideoW, *VideoH, codecCtx->pix_fmt, *VideoW / 2, *VideoH / 2, AV_PIX_FMT_YUV420P, SWS_BILINEAR, nullptr, nullptr, nullptr);

    if(swsCtx == nullptr) {
        return -1;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, *VideoW / 2, *VideoH / 2);

    if(texture == nullptr) {
        return -1;
    }

    int bufferSize = av_image_get_buffer_size(AV_PIX_FMT_YUV420P, *VideoW / 2, *VideoH / 2, 1);

    uint8_t* buffer = (uint8_t*)av_malloc(bufferSize);

    if(buffer == nullptr) {
        return -1;
    }

    return_code = av_image_fill_arrays(yuvFrame->data, yuvFrame->linesize, buffer, AV_PIX_FMT_YUV420P, *VideoW / 2, *VideoH / 2, 1);

    if(return_code < 0) {
        return -1;
    }

    return 0;
}