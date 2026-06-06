#define SDL_MAIN_HANDLED
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

#include <SDL2/SDL.h>
#include <iostream>

int main() {

    const char* filename = "assets/videos/Video.mp4";

    AVFormatContext* formatCtx = nullptr;
    if (avformat_open_input(&formatCtx, filename, nullptr, nullptr) != 0) {
        std::cout << "Không mở được video\n";
        return -1;
    }

    avformat_find_stream_info(formatCtx, nullptr);

    int videoStream = -1;
    for (unsigned i = 0; i < formatCtx->nb_streams; i++) {
        if (formatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStream = i;
            break;
        }
    }

    if (videoStream == -1) {
        std::cout << "Không tìm thấy stream video\n";
        return -1;
    }

    AVCodecParameters* codecpar = formatCtx->streams[videoStream]->codecpar;
    const AVCodec* codec = avcodec_find_decoder(codecpar->codec_id);

    AVCodecContext* codecCtx = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(codecCtx, codecpar);
    avcodec_open2(codecCtx, codec, nullptr);

    int width = codecCtx->width;
    int height = codecCtx->height;

    std::cout << "Video size: " << width / 2 << "x" << height / 2 << "\n";

        SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "Video Player",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width / 2, height / 2,
        0
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_Texture* texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_YV12,
        SDL_TEXTUREACCESS_STREAMING,
        width / 2, height / 2
    );

        
    SwsContext* swsCtx = sws_getContext(
        width, height, codecCtx->pix_fmt,
        width / 2, height / 2, AV_PIX_FMT_YUV420P,
        SWS_BILINEAR, nullptr, nullptr, nullptr
    );

    AVFrame* frame = av_frame_alloc();
    AVFrame* yuvFrame = av_frame_alloc();

    uint8_t* buffer = (uint8_t*)av_malloc(
        av_image_get_buffer_size(AV_PIX_FMT_YUV420P, width / 2, height / 2, 1)
    );

    av_image_fill_arrays(
        yuvFrame->data, yuvFrame->linesize,
        buffer,
        AV_PIX_FMT_YUV420P,
        width / 2, height / 2, 1
    );

    AVPacket packet;
    SDL_Event event;
    bool running = true;

    while (running && av_read_frame(formatCtx, &packet) >= 0) {

        if (packet.stream_index == videoStream) {

            avcodec_send_packet(codecCtx, &packet);

            while (avcodec_receive_frame(codecCtx, frame) == 0) {

                sws_scale(
                    swsCtx,
                    frame->data,
                    frame->linesize,
                    0,
                    height,
                    yuvFrame->data,
                    yuvFrame->linesize
                );

                SDL_UpdateYUVTexture(
                    texture,
                    nullptr,
                    yuvFrame->data[0], yuvFrame->linesize[0],
                    yuvFrame->data[1], yuvFrame->linesize[1],
                    yuvFrame->data[2], yuvFrame->linesize[2]
                );

                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, texture, nullptr, nullptr);
                SDL_RenderPresent(renderer);

                SDL_Delay(16); // ~30fps
            }
        }

        av_packet_unref(&packet);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;
        }
    }

    SDL_Quit();
    return 0;
}