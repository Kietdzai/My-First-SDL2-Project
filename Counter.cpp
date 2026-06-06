#include "Counter.h"
// Hàm này có tác dụng sẽ tạo lại texture mới khi cái số thay đổi (texture chữ) của Counter
void Update_CreateTexture(SDL_Renderer* renderer, Counter& ui, TTF_Font*& font){
    if (!ui.NeedToChange) return;
    if (ui.texture){
        SDL_DestroyTexture(ui.texture);
        ui.texture = nullptr;
    }
    const char* text = std::to_string(ui.so).c_str();
    RenderTextToTexture(renderer, ui.texture, font, text, ui.TextColor);

    SDL_QueryTexture(ui.texture, nullptr, nullptr, &ui.TextRenderArea.w, &ui.TextRenderArea.h);
    ui.TextRenderArea.y = 26 - ui.TextRenderArea.h / 2;
}
