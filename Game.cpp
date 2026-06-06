#include "Game.h"
___MAPTYPE___ questions = {}; // Bộ câu hỏi

void FirstLoad(Question& question){
    Load(::questions);

    std::string randomize_ques = RandomQuestion(questions);
    question.question = randomize_ques; // Random câu hỏi và đưa nó vào trong biến Question
    question.NeedToChange = true;
}
