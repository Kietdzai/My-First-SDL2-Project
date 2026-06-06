#include "Video.h"
// hàm này chạy tất cả những gì cần làm trước khi render video
int FullLoadVideo(SDL_Renderer*& renderer, SDL_Texture*& texture, AVFormatContext*& formatCtx, AVCodecContext*& codecCtx, AVFrame*& frame, AVFrame*& yuvFrame, SwsContext*& swsCtx, int& videoStream, const char*& path, int* VideoW, int* VideoH){
    // Mở file
    if (avformat_open_input(&formatCtx, path, nullptr, nullptr) != 0) {
        return -1;
    }

    // Lấy thông tin stream
    avformat_find_stream_info(formatCtx, nullptr);

    // Tìm stream video
    for (unsigned i = 0; i < formatCtx->nb_streams; i++) {
        if (formatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStream = i;
            break;
        }
    }

    if (videoStream == -1) return -1;

    // Lấy codec
    AVCodecParameters* codecpar = formatCtx->streams[videoStream]->codecpar;
    const AVCodec* codec = avcodec_find_decoder(codecpar->codec_id);

    codecCtx = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(codecCtx, codecpar);
    avcodec_open2(codecCtx, codec, nullptr);

    *VideoW = codecCtx->width;
    *VideoH = codecCtx->height;
        
    swsCtx = sws_getContext(*VideoW, *VideoH, codecCtx->pix_fmt, *VideoW / 2, *VideoH / 2, AV_PIX_FMT_YUV420P, SWS_BILINEAR, nullptr, nullptr, nullptr);

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING, *VideoW / 2, *VideoH / 2);

    frame = av_frame_alloc();
    yuvFrame = av_frame_alloc();

    uint8_t* buffer = static_cast<uint8_t*>(av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P, *VideoW / 2, *VideoH / 2, 1)));

    av_image_fill_arrays(yuvFrame->data, yuvFrame->linesize, buffer, AV_PIX_FMT_YUV420P, *VideoW / 2, *VideoH / 2, 1);
}