#pragma once
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Font.h"
#include "Button.h"

class Answer{
public:
    std::string cautraloi = "";
    bool Choosing = false; // Đánh dấu khi chọn câu trả lời này (chưa nhấn Ok), mỗi một câu hỏi đều có duy nhất 1 câu trả lời
    SDL_Texture* texture = nullptr;
    SDL_Texture* UITexture = nullptr;
    SDL_Rect RenderArea = {};
    Button UI;
};
void UpdateAnswerTexture(SDL_Renderer* renderer, Answer& ans, TTF_Font*& font, const int& height);