#include "Answer.h"

void UpdateAnswerTexture(SDL_Renderer* renderer, Answer& ans, TTF_Font*& font, const int& height) {
    // Cần có chiều cao để căn giữa màn hình theo trục y
    if (ans.texture) {
        SDL_DestroyTexture(ans.texture);
        ans.texture = nullptr;
    }

    SDL_Color black = {0, 0, 0, 255};

    const char* text = ans.cautraloi.c_str();
    RenderTextToTexture(renderer, ans.texture, font, text, black);

    SDL_QueryTexture(ans.texture, nullptr, nullptr, &ans.RenderArea.w, &ans.RenderArea.h);

    ans.RenderArea.y = (height - ans.RenderArea.h) / 2 - 100 + ans.RenderArea.h + 10; // Vị trí ngay dưới câu hỏi
}