#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class HealthBar {
public:
    /* 6 trạng thái là gồm có 5 máu và đã thua (0 máu). Nếu index = 1 thì tức là 1 máu, ... */
    int TextureSizeX = 0;
    int TextureSizeY = 0;
    const int ButtonSizeX = 375;
    const int ButtonSizeY = 56;
    const int DistFromButtonY = 23; // Mội cột trong ảnh có các thanh máu, mỗi thanh đó cách 23 px
    SDL_Rect Button[6];
    SDL_Rect RenderArea;
    SDL_Texture* texture = nullptr;

    int HealthBarState = 0; // số máu còn lại
    // Hàm này giúp cắt ảnh
    void Clip(){
        for (int trangthai = 0; trangthai < 6; trangthai++){
            if (trangthai == 0 || trangthai == 1 || trangthai == 2) {
                Button[trangthai].x = 18 + ButtonSizeX + 42;
                Button[trangthai].y = TextureSizeY - (45 + ButtonSizeY + ((trangthai) * (DistFromButtonY + ButtonSizeY)));
            }
            if (trangthai == 3 || trangthai == 4 || trangthai == 5) {
                Button[trangthai].x = 18;
                Button[trangthai].y = TextureSizeY - (45 + ButtonSizeY + ((trangthai - 3) * (DistFromButtonY + ButtonSizeY)));
            }

            Button[trangthai].w = ButtonSizeX;
            Button[trangthai].h = ButtonSizeY;
        }
    }

    void GetWHTexture(){SDL_QueryTexture(texture, nullptr, nullptr, &TextureSizeX, &TextureSizeY);}
};