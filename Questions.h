#pragma once
#include <variant>
#include <map>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <cstdlib>
#include "JSONObject.h"
#include "Font.h"

void Load(___MAPTYPE___& questions);
bool IsAnswerTrue(___MAPTYPE___& questions, const std::string& question, const std::string& answer);
std::string RandomQuestion(const ___MAPTYPE___& questions);
class Question {
public:
    std::string question = "";
    SDL_Rect RenderArea = {};
    bool NeedToChange = true;
    void SetRenderArea(SDL_Rect& area){RenderArea = area;}
    void ChangeQuestion(const std::string& ques){
        question = ques;
        NeedToChange = true;
    }
    SDL_Texture* texture = nullptr;
};

int UpdateQuestionTexture(SDL_Renderer* renderer, Question& ques, TTF_Font*& font, const int& width, const int& height);