#include "Questions.h"

void Load(___MAPTYPE___& questions){
    ntk::JSONObject obj("assets/question/questions.json");
    obj.open();
    questions = obj.dump();
    // obj.SeeInsideThisObject();
}
bool IsAnswerTrue(___MAPTYPE___& questions, const std::string& question, const std::string& answer){
    /* Cách check:
    đầu tiên, ta có ___MAPTYPE___ = std::variant<...>
    std::variant chứa tất cả các thể loại dữ liệu đưa vào, rất linh hoạt
    Khi trích xuất một cái dữ liệu (gọi là key) thì nó sẽ ra giá trị (gọi là value). và nó thuộc kiểu std::variant
    Tuy nhiên, nó chứa một kiểu dữ liệu và không thể chạy hàm, các toán tử nạp chồng bên trong nó (ngoại trừ việc gán)
    ví dụ, nếu gọi std::variant<std::string> chuoi = 1;
    chuoi.size(); thì ra lỗi
    cách fix: ép về kiểu gốc của nó rồi mới dùng
    */
    return ConvertVariantToString(ConvertVariantToMap(questions[question])["Answer"]) == answer;
}

std::string RandomQuestion(const ___MAPTYPE___& questions){
    // Code cũ
    // Độ phức tạp: O(n)
    // std::vector<std::string> cauhoi;
    // for (const auto& pair : questions){
    //     cauhoi.push_back(pair.first);
    // }
    // unsigned long long int randomize_index = std::rand() % cauhoi.size();
    // std::string ques = cauhoi[randomize_index];
    // std::cout << "Câu hỏi được random là: " << ques << std::endl;

    // return ques;

    // Code mới
    // Độ phức tạp: O(n), tuy nhiên đỡ tốn RAM
    unsigned long long random_index = std::rand() % questions.size();
    auto iterator = questions.begin(); // Con trỏ iterator
    std::advance(iterator, random_index); // di chuyển con trỏ iterator lên random_index bước - tức là di chuyển lên cặp được random

    std::string ques = iterator-> first;
    return ques;
}
// Hàm này có tác dụng là tạo lại texture mới khi câu hỏi thay đổi
int UpdateQuestionTexture(SDL_Renderer* renderer, Question& ques, TTF_Font*& font, const int& width, const int& height) {
    // Cần thêm chiều rộng và chiều cao để căn giữa màn hình
    if (!ques.NeedToChange) return 0; // Trả về 0 tức là không thay đổi

    if (ques.texture) {
        SDL_DestroyTexture(ques.texture);
        ques.texture = nullptr;
    }

    SDL_Color black = {0, 0, 0, 255};

    const char* text = ques.question.c_str();
    RenderTextToTexture(renderer, ques.texture, font, text, black);

    SDL_QueryTexture(ques.texture, nullptr, nullptr, &ques.RenderArea.w, &ques.RenderArea.h);
    ques.RenderArea.x = (width - ques.RenderArea.w) / 2; // Căn giữa màn hình
    ques.RenderArea.y = (height - ques.RenderArea.h) / 2 - 100;
    ques.NeedToChange = false;
    return 1;
}