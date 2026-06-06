#pragma once
#include <SDL2/SDL.h>

class MenuButton {
public:
    SDL_Rect Button[3]; // Vị trí của nút trong 3 trạng thái, được lưu vị trí trên ảnh
    SDL_Rect ButtonRenderArea; // Vị trí hiển thị của nút (Cùng 1 chỗ trong cả ba trạng thái: bình thường, rê chuột vào và nhấn)
    int ButtonState = 0; // Quy ước: 0 là khi rê chuột vào, 1 là bình thường, 2 là khi nhấn vào
    bool MouseInButton = false; // Đánh dấu xem chuột có ở trong nút không
    bool Pressed = false; // Cái này đánh dấu để cho "đẹp"
};

class Button {
public:
    SDL_Rect Button[2]; // Vị trí của nút trong 2 trạng thái: bình thường và đã nhấn trên ảnh;
    SDL_Rect ButtonRenderArea; // Vị trí của nút đó
    int ButtonState = 0; // Quy ước: 0 là bình thường, 1 là khi nhấn vào
    bool MouseInButton = false; // Đánh dấu xem chuột có ở trong nút không
    bool Pressed = false;
};

class Counter {
public:
    unsigned long long so = 0;
    SDL_Texture* texture = nullptr;
    SDL_Texture* icon = nullptr;

    SDL_Rect TextRenderArea = {};
    SDL_Rect UIRenderArea = {};
    SDL_Color TextColor = {};

    bool NeedToChange = true;
};