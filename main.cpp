#define SDL_MAIN_HANDLED
#include "Window.h"
#include "Questions.h"
#include "Cursor.h"
#include "LoadImage.h"
#include "Answer.h"
#include "Font.h"
#include "Game.h"
#include "Button.h"
#include "Counter.h"
#include "Sound.h"
#include "HealthBar.h"
#include "Video.h"

#include <iostream>
#include <cstring>

int main(int argc, const char** argv){
    SetUnicodeForTheOutput();
    bool run = true;
    bool end = false;
    SDL_Window* win;
    const int w = 1280, h = 720;
    const int MenuButtonSizeX = 192, MenuButtonSizeY = 96; // Kích thước của các nút đầu Game (phần Menu)
    const int SubmitButtonSizeX = 223, SubmitButtonSizeY = 83;
    const int AnswerButtonSizeX = 1524, AnswerButtonSizeY = 126;
    
    const char* window_name = "Làng bánh khô mè Bà Liễu Mẹ Thành Phố Đà Nẵng";

    SDL_Texture* VideoTexture = nullptr;
    AVFormatContext* formatCtx = nullptr;
    AVCodecContext* codecCtx = nullptr;
    AVFrame* frame = nullptr;
    AVFrame* yuvFrame = nullptr;
    SwsContext* swsCtx = nullptr;
    int videoStream = -1;
    const char* round1path = "assets/videos/Video.mp4";
    int VideoW, VideoH;

    // Khởi tạo
    if (StartWindow(win, window_name, w, h) != 0) {
        std::cout << "Đã có lỗi xảy ra trong lúc tạo cửa sổ SDL. Chi tiết: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    SDL_Renderer* renderer;
    if (Renderer(win, renderer) != 0) {
        std::cout << "Đã có lỗi xảy ra trong lúc khởi tạo Renderer. Chi tiết: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }
    if (InitImage() != 0){
        std::cout << "Đã có lỗi xảy ra trong lúc khởi tạo ảnh (xuất phát từ SDL_image). Chi tiết: " << IMG_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }
    if (InitFont() != 0){
        std::cout << "Đã có lỗi trong lúc khởi tạo Font (xuất phát từ SDL_ttf). Chi tiết: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(win);
        QuitImage();
        SDL_Quit();
        return 1;
    }
    if (FirstStartAudio() != 0){
        std::cout << "Đã có lỗi xảy ra trong lúc khởi tạo âm thanh (xuất phát từ SDL_mixer). Chi tiết: " << Mix_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(win);
        QuitImage();
        QuitFont();
        SDL_Quit();
        return 1;
    }
    if (InitSound() != 0){
        std::cout << "Đã có lỗi xảy ra trong lúc khởi tạo âm thanh (xuất phát từ SDL_mixer). Chi tiết: " << Mix_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(win);
        QuitImage();
        QuitFont();
        SoundCleanup();
        SDL_Quit();
        return 1;
    }

    SDL_Texture* All_Menu_Button = nullptr;
    SDL_Texture* Submit_Button_Texture = nullptr;
    SDL_Texture* MenuBackground = nullptr;
    SDL_Texture* GameBackground = nullptr;
    // Vị trí render
    SDL_Rect MenuBackgroundRenderArea = {0, 0, w, h};
    SDL_Rect GameBackgroundRenderArea = {0, 0, w, h};
    // Khởi tạo câu trả lời
    Answer CauA;
    Answer CauB;
    Answer CauC;
    Answer CauD;
    // Khởi tạo bộ đếm
    Counter AnsCounter;
    Counter TrueCounter;
    // Khởi tạo thanh máu
    HealthBar health;

    // Đường dẫn tới tài nguyên cần thiết
    const char* MenuButtonPath = "assets/textures/MenuButton.png";
    const char* SubmitButtonPath = "assets/textures/SubmitButton.png";
    const char* MenuBackgroundPath = "assets/textures/MenuBackground.png";
    const char* GameBackgroundPath = "assets/textures/GameBackground.png";
    const char* AnswerUIPath = "assets/textures/Answer.png";
    const char* FontPath = "assets/fonts/TimesNewRoman.ttf";
    const char* CorrectSoundEffectPath = "assets/sounds/correct.wav";
    const char* IncorrectSoundEffectPath = "assets/sounds/incorrect.wav";
    const char* HealthBarPath = "assets/textures/healthbar.png";

    if (LoadTexture(All_Menu_Button, renderer, MenuButtonPath) == 1){
        std::cout << "Đã có lỗi trong lúc tạo Texture cho các nút ở Menu. Chi tiết: " << IMG_GetError() << std::endl;
        // Giải phóng bộ nhớ
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(win);
        QuitFont();
        QuitImage();
        SoundCleanup();
        SDL_Quit();
        return 1;
    }
    if (LoadTexture(Submit_Button_Texture, renderer, SubmitButtonPath) == 1){
        std::cout << "Đã có lỗi trong lúc tạo Texture cho nút OK ở phần Game. Chi tiết: " << IMG_GetError() << std::endl;
        // Giải phóng bộ nhớ
        SDL_DestroyTexture(All_Menu_Button);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(win);
        QuitFont();
        QuitImage();
        SoundCleanup();
        SDL_Quit();
        return 1;
    }
    if (LoadTexture(MenuBackground, renderer, MenuBackgroundPath) == 1){
        std::cout << "Đã có lỗi trong lúc tạo Texture cho hình nền của phần Menu. Chi tiết: " << IMG_GetError() << std::endl;
        // Giải phóng bộ nhớ
        SDL_DestroyTexture(All_Menu_Button);
        SDL_DestroyTexture(Submit_Button_Texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(win);
        QuitFont();
        QuitImage();
        SoundCleanup();
        SDL_Quit();
        return 1;
    }
    if (LoadTexture(GameBackground, renderer, GameBackgroundPath) == 1) {
        std::cout << "Đã có lỗi trong lúc tạo Texture cho hình nền của phần Game. Chi tiết: " << IMG_GetError() << std::endl;
        // Giải phóng bộ nhớ
        SDL_DestroyTexture(MenuBackground);
        SDL_DestroyTexture(All_Menu_Button);
        SDL_DestroyTexture(Submit_Button_Texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(win);
        QuitFont();
        QuitImage();
        SoundCleanup();
        SDL_Quit();
        return 1;
    }
    if (LoadTexture(CauA.UITexture, renderer, AnswerUIPath) == 1){
        std::cout << "Đã có lỗi trong lúc tạo Texture cho nút trả lời ở phần Game. Chi tiết: " << IMG_GetError() << std::endl;
        // Giải phóng bộ nhớ
        SDL_DestroyTexture(All_Menu_Button);
        SDL_DestroyTexture(Submit_Button_Texture);
        SDL_DestroyTexture(MenuBackground);
        SDL_DestroyTexture(GameBackground);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(win);
        QuitFont();
        QuitImage();
        SoundCleanup();
        SDL_Quit();
        return 1;
    }
    CauB.UITexture = CauA.UITexture;
    CauC.UITexture = CauA.UITexture;
    CauD.UITexture = CauA.UITexture;
    
    Mix_Chunk* CorrectSoundEffect;
    Mix_Chunk* IncorrectSoundEffect;
    Mix_Chunk* HitSoundEffect;
    if (LoadChunk(CorrectSoundEffect, CorrectSoundEffectPath) == 1){
        std::cout << "Đã có lỗi xảy ra trong lúc load nhạc (sound effect) của 'correct.wav'. Chi tiết: " << Mix_GetError() << std::endl;
        // Giải phóng bộ nhớ
        SDL_DestroyTexture(All_Menu_Button);
        SDL_DestroyTexture(Submit_Button_Texture);
        SDL_DestroyTexture(MenuBackground);
        SDL_DestroyTexture(GameBackground);
        SDL_DestroyTexture(CauA.UITexture);
        SDL_DestroyTexture(CauB.UITexture);
        SDL_DestroyTexture(CauC.UITexture);
        SDL_DestroyTexture(CauD.UITexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(win);
        QuitFont();
        QuitImage();
        SoundCleanup();
        SDL_Quit();
        return 1;
    }
    if (LoadChunk(IncorrectSoundEffect, IncorrectSoundEffectPath) == 1){
        std::cout << "Đã có lỗi xảy ra trong lúc load nhạc (sound effect) của 'incorrect.wav'. Chi tiết: " << Mix_GetError() << std::endl;
        // Giải phóng bộ nhớ
        SDL_DestroyTexture(All_Menu_Button);
        SDL_DestroyTexture(Submit_Button_Texture);
        SDL_DestroyTexture(MenuBackground);
        SDL_DestroyTexture(GameBackground);
        SDL_DestroyTexture(CauA.UITexture);
        SDL_DestroyTexture(CauB.UITexture);
        SDL_DestroyTexture(CauC.UITexture);
        SDL_DestroyTexture(CauD.UITexture);
        Mix_FreeChunk(CorrectSoundEffect);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(win);
        QuitFont();
        QuitImage();
        SoundCleanup();
        SDL_Quit();
        return 1;
    }
    if (LoadTexture(health.texture, renderer, HealthBarPath) == 1) {
        std::cout << "Đã có lỗi xảy ra trong lúc tạo Texture cho thanh máu. Chi tiết: " << IMG_GetError() << std::endl;
        // Giải phóng bộ nhớ
        SDL_DestroyTexture(All_Menu_Button);
        SDL_DestroyTexture(Submit_Button_Texture);
        SDL_DestroyTexture(MenuBackground);
        SDL_DestroyTexture(GameBackground);
        SDL_DestroyTexture(CauA.UITexture);
        SDL_DestroyTexture(CauB.UITexture);
        SDL_DestroyTexture(CauC.UITexture);
        SDL_DestroyTexture(CauD.UITexture);
        Mix_FreeChunk(CorrectSoundEffect);
        Mix_FreeChunk(IncorrectSoundEffect);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(win);
        QuitFont();
        QuitImage();
        SoundCleanup();
        SDL_Quit();
        return 1;
    }

    if (FullLoadVideo(renderer, VideoTexture, formatCtx, codecCtx, frame, yuvFrame, swsCtx, videoStream, round1path, &VideoW, &VideoH) == -1){
        std::cout << "Đã có lỗi trong lúc khởi tạo video." << std::endl;
        // Giải phóng bộ nhớ
        SDL_DestroyTexture(All_Menu_Button);
        SDL_DestroyTexture(Submit_Button_Texture);
        SDL_DestroyTexture(MenuBackground);
        SDL_DestroyTexture(GameBackground);
        SDL_DestroyTexture(CauA.UITexture);
        SDL_DestroyTexture(CauB.UITexture);
        SDL_DestroyTexture(CauC.UITexture);
        SDL_DestroyTexture(CauD.UITexture);
        SDL_DestroyTexture(health.texture);
        Mix_FreeChunk(CorrectSoundEffect);
        Mix_FreeChunk(IncorrectSoundEffect);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(win);
        QuitFont();
        QuitImage();
        SoundCleanup();
        SDL_Quit();
        return 1;
    }

    int TrueAns = 0; // NẾU trả lời đúng 5 lần thì được tăng round ở dưới
    int round = 0; // NẾU round tăng thì đoạn video xem được cũng tăng

    AVPacket packet;
    SDL_Event event;
    int x = 0, y = 0;
    // Nút Play
    MenuButton Play;

    // Nút Thoát
    MenuButton Exit;

    // Nút Ok (khi vào trong game)
    Button Submit;
    // Trạng thái của chuột
    SDL_Cursor* normal = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    SDL_Cursor* clickable = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);

    char where[5] = "menu"; // Đánh dấu đang ở đâu, ở menu hay ở trong game
    // Cấu hình nút Play
    // Cắt ảnh ra thành từng trạng thái của nút Play và lưu vị trí của nó trên ảnh
    for (int trangthai = 0; trangthai < 3; trangthai++){
        Play.Button[trangthai].x = trangthai * MenuButtonSizeX;
        Play.Button[trangthai].y = 0;
        Play.Button[trangthai].w = MenuButtonSizeX;
        Play.Button[trangthai].h = MenuButtonSizeY;
    }
    // Vị trí của nút Play
    Play.ButtonRenderArea.x = w / 2 - MenuButtonSizeX / 2;
    Play.ButtonRenderArea.y = h / 2 - MenuButtonSizeY / 2;
    Play.ButtonRenderArea.w = MenuButtonSizeX;
    Play.ButtonRenderArea.h = MenuButtonSizeY;

    // Cấu hình nút thoát
    // Cắt ảnh ra thành từng trạng thái của nút Thoát và lưu vị trí của nó trên ảnh
    for (int trangthai = 0; trangthai < 3; trangthai++){
        Exit.Button[trangthai].x = trangthai * MenuButtonSizeX;
        Exit.Button[trangthai].y = MenuButtonSizeY;
        Exit.Button[trangthai].w = MenuButtonSizeX;
        Exit.Button[trangthai].h = MenuButtonSizeY;
    }
    // Vị trí của nút thoát
    Exit.ButtonRenderArea.x = w / 2 - MenuButtonSizeX / 2;
    Exit.ButtonRenderArea.y = h / 2 - MenuButtonSizeY / 2 + MenuButtonSizeY + 10;
    Exit.ButtonRenderArea.w = MenuButtonSizeX;
    Exit.ButtonRenderArea.h = MenuButtonSizeY;

    // Cấu hình nút Submit
    // Cắt ảnh
    for (int trangthai = 0; trangthai < 2; trangthai++){
        Submit.Button[trangthai].x = 0;
        if (trangthai == 0) Submit.Button[trangthai].y = 0;
        if (trangthai == 1) Submit.Button[trangthai].y = SubmitButtonSizeY + 25; // Khoảng cách trống giữa 2 cái nút là 25px
        Submit.Button[trangthai].w = SubmitButtonSizeX;
        Submit.Button[trangthai].h = SubmitButtonSizeY;
    }
    // Vị trí được render
    Submit.ButtonRenderArea.x = w / 2 - SubmitButtonSizeX / 4;
    Submit.ButtonRenderArea.y = h - 70; // Dù với mọi h, thì nút vẫn cách viền cửa sổ dưới 70 px
    Submit.ButtonRenderArea.w = SubmitButtonSizeX / 2;
    Submit.ButtonRenderArea.h = SubmitButtonSizeY / 2;

    // Cáu hình nút trả lời A
    // Cắt ảnh
    for (int trangthai = 0; trangthai < 2; trangthai++){
        CauA.UI.Button[trangthai].x = 0;
        CauA.UI.Button[trangthai].w = AnswerButtonSizeX;
    }
    CauA.UI.Button[0].y = AnswerButtonSizeY - 2; // chỉnh ảnh bay mất 2 px
    CauA.UI.Button[0].h = AnswerButtonSizeY;
    CauA.UI.Button[1].y = 0;
    CauA.UI.Button[1].h = AnswerButtonSizeY - 4;
    // Vị trí được render
    CauA.UI.ButtonRenderArea.x = w / 2 - AnswerButtonSizeX / 4;

    CauA.UI.ButtonRenderArea.w = AnswerButtonSizeX / 2;
    CauA.UI.ButtonRenderArea.h = AnswerButtonSizeY / 2;

    // Cấu hình nút trả lời B
    // Cắt ảnh y chang câu A
    for (int trangthai = 0; trangthai < 2; trangthai++){
        CauB.UI.Button[trangthai] = CauA.UI.Button[trangthai];
    }
    CauB.UI.ButtonRenderArea = CauA.UI.ButtonRenderArea;
    
    // Cấu hình nút trả lời C
    for (int trangthai = 0; trangthai < 2; trangthai++){
        CauC.UI.Button[trangthai] = CauA.UI.Button[trangthai];
    }
    CauC.UI.ButtonRenderArea = CauB.UI.ButtonRenderArea;
    // Cấu hình nút trả lời D
    for (int trangthai = 0; trangthai < 2; trangthai++){
        CauD.UI.Button[trangthai] = CauA.UI.Button[trangthai];
    }
    CauD.UI.ButtonRenderArea = CauC.UI.ButtonRenderArea;

    TTF_Font* font;
    LoadFont(FontPath, 20, font);

    // Khởi tạo Câu hỏi
    Question question;

    // Set từng câu trả lời vào các biến
    // Định nghĩa
    FirstLoad(question); // Lúc này câu hỏi đã được Random

    // Set từng câu trả lời vào các biến
    ___VALUETYPE___ AnsDataVariant = questions[question.question];
    ___MAPTYPE___ AnsDataMap = ConvertVariantToMap(AnsDataVariant);

    // Gán vào mỗi câu hỏi
    for (auto& pair : AnsDataMap){
        if (pair.first == "A") CauA.cautraloi = ConvertVariantToString(pair.second);

        if (pair.first == "B") CauB.cautraloi = ConvertVariantToString(pair.second);

        if (pair.first == "C") CauC.cautraloi = ConvertVariantToString(pair.second);

        if (pair.first == "D") CauD.cautraloi = ConvertVariantToString(pair.second);
    }

    question.RenderArea = {50, 400};

    // Set vị trí x của mỗi câu trả lời
    CauA.RenderArea.x = CauA.UI.ButtonRenderArea.x + 75;
    CauB.RenderArea.x = CauA.RenderArea.x;
    CauC.RenderArea.x = CauA.RenderArea.x;
    CauD.RenderArea.x = CauA.RenderArea.x;

    // Cài đặt bộ đếm
    AnsCounter.TextColor = {0, 0, 0, 255};
    AnsCounter.so = questions.size();
    AnsCounter.TextRenderArea.x = 1225;

    TrueCounter.TextColor = {0, 0, 0, 255};
    TrueCounter.so = 0;
    TrueCounter.TextRenderArea.x = 1120;

    Update_CreateTexture(renderer, AnsCounter, font);
    Update_CreateTexture(renderer, TrueCounter, font);

    // Vị trí render của thanh máu
    health.GetWHTexture();
    health.RenderArea.x = 25;
    health.RenderArea.y = 10;
    health.RenderArea.w = health.ButtonSizeX / 2; // Thu nhỏ kích thước
    health.RenderArea.h = health.ButtonSizeY / 2; // Thu nhỏ kích thước
    // Trạng thái bắt đầu của thanh máu là full
    health.HealthBarState = 5;
    // Cắt ảnh
    health.Clip();
    // Đánh dấu người chơi trả lời đúng hay sai
    bool AnswerTrue = false;
    while (run){
        SDL_GetMouseState(&x, &y);
        if (std::strcmp(where, "menu") == 0){
            if (PointInRect(x, y, Play.ButtonRenderArea)) {
                ChangeCursor(clickable);
                Play.ButtonState = 0;
                Play.MouseInButton = true;
                Exit.MouseInButton = false;
            } if (!PointInRect(x, y, Play.ButtonRenderArea)) {
                ChangeCursor(normal);
                Play.ButtonState = 1;
                Play.MouseInButton = false;
            }

            if (PointInRect(x, y, Exit.ButtonRenderArea)) {
                ChangeCursor(clickable);
                Exit.ButtonState = 0;
                Exit.MouseInButton = true;
                Play.MouseInButton = false;
            } if (!PointInRect(x, y, Exit.ButtonRenderArea)) {
                if (!PointInRect(x, y, Play.ButtonRenderArea)) ChangeCursor(normal);
                Exit.ButtonState = 1;
                Exit.MouseInButton = false;
            }
        }

        else if (std::strcmp(where, "game") == 0){
            if (PointInRect(x, y, Submit.ButtonRenderArea)) {
                ChangeCursor(clickable);
                Submit.ButtonState = 0; // trở về trạng thái mặc định
                Submit.MouseInButton = true;
            }
            if (!PointInRect(x, y, Submit.ButtonRenderArea)) {
                ChangeCursor(normal);
                Submit.MouseInButton = false;
            }

            if (PointInRect(x, y, CauA.UI.ButtonRenderArea)){
                ChangeCursor(clickable);
                CauA.UI.MouseInButton = true;
            }
            if (!PointInRect(x, y, CauA.UI.ButtonRenderArea)){
                if (!PointInRect(x, y, Submit.ButtonRenderArea) && !PointInRect(x, y, CauB.UI.ButtonRenderArea)) ChangeCursor(normal);
                CauA.UI.MouseInButton = false;
            }

            if (PointInRect(x, y, CauB.UI.ButtonRenderArea)){
                ChangeCursor(clickable);
                CauB.UI.MouseInButton = true;
            }
            if (!PointInRect(x, y, CauB.UI.ButtonRenderArea)){
                if (!PointInRect(x, y, Submit.ButtonRenderArea) && !PointInRect(x, y, CauA.UI.ButtonRenderArea)) ChangeCursor(normal);
                CauB.UI.MouseInButton = false;
            }

            if (PointInRect(x, y, CauC.UI.ButtonRenderArea)){
                ChangeCursor(clickable);
                CauC.UI.MouseInButton = true;
            }
            if (!PointInRect(x, y, CauC.UI.ButtonRenderArea)){
                if (!PointInRect(x, y, Submit.ButtonRenderArea) && !PointInRect(x, y, CauA.UI.ButtonRenderArea) && !PointInRect(x, y, CauB.UI.ButtonRenderArea)) ChangeCursor(normal);
                CauC.UI.MouseInButton = false;
            }

            if (PointInRect(x, y, CauD.UI.ButtonRenderArea)){
                ChangeCursor(clickable);
                CauD.UI.MouseInButton = true;
            }
            if (!PointInRect(x, y, CauD.UI.ButtonRenderArea)){
                if (!PointInRect(x, y, Submit.ButtonRenderArea) && !PointInRect(x, y, CauA.UI.ButtonRenderArea) && !PointInRect(x, y, CauB.UI.ButtonRenderArea) && !PointInRect(x, y, CauC.UI.ButtonRenderArea)) ChangeCursor(normal);
                CauD.UI.MouseInButton = false;
            }
        }
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT) {
                std::cout << "Đã nhấn nút 'X' hoặc Ctrl + C ở cmd. Đang dọn dẹp bộ nhớ." << std::endl;
                run = false;
                break;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN){
                if (event.button.button == SDL_BUTTON_LEFT){
                    if (std::strcmp(where, "menu") == 0){
                        if (Play.MouseInButton){
                            Play.ButtonState = 2;
                            Play.Pressed = true;
                        } else if (Exit.MouseInButton){
                            Exit.ButtonState = 2;
                            Exit.Pressed = true;
                        }
                    } if (std::strcmp(where, "game") == 0){
                        if (Submit.MouseInButton){
                            Submit.ButtonState = 1;
                            Submit.Pressed = true;
                        } else if (CauA.UI.MouseInButton){
                            if (CauB.UI.Pressed){
                                CauB.UI.ButtonState = 0;
                                CauB.UI.Pressed = false;
                                CauB.Choosing = false;
                            } else if (CauC.UI.Pressed){
                                CauC.UI.ButtonState = 0;
                                CauC.UI.Pressed = false;
                                CauC.Choosing = false;
                            } else if (CauD.UI.Pressed){
                                CauD.UI.ButtonState = 0;
                                CauD.UI.Pressed = false;
                                CauD.Choosing = false;
                            }
                            CauA.UI.ButtonState = 1;
                            CauA.Choosing = true;
                            CauA.UI.Pressed = true;
                        } else if (CauB.UI.MouseInButton){
                            //  Chỉ có duy nhất 1 câu trả lời
                            if (CauA.UI.Pressed){
                                CauA.UI.ButtonState = 0;
                                CauA.UI.Pressed = false;
                                CauA.Choosing = false;
                            } else if (CauC.UI.Pressed){
                                CauC.UI.ButtonState = 0;
                                CauC.UI.Pressed = false;
                                CauC.Choosing = false;
                            } else if (CauD.UI.Pressed){
                                CauD.UI.ButtonState = 0;
                                CauD.UI.Pressed = false;
                                CauD.Choosing = false;
                            }
                            CauB.UI.ButtonState = 1;
                            CauB.Choosing = true;
                            CauB.UI.Pressed = true;
                        } else if (CauC.UI.MouseInButton){
                            //  Chỉ có duy nhất 1 câu trả lời
                            if (CauA.UI.Pressed){
                                CauA.UI.ButtonState = 0;
                                CauA.UI.Pressed = false;
                                CauA.Choosing = false;
                            } else if (CauB.UI.Pressed){
                                CauB.UI.ButtonState = 0;
                                CauB.UI.Pressed = false;
                                CauB.Choosing = false;
                            } else if (CauD.UI.Pressed){
                                CauD.UI.ButtonState = 0;
                                CauD.UI.Pressed = false;
                                CauD.Choosing = false;
                            }
                            CauC.UI.ButtonState = 1;
                            CauC.Choosing = true;
                            CauC.UI.Pressed = true;
                        } else if (CauD.UI.MouseInButton){
                            //  Chỉ có duy nhất 1 câu trả lời
                            if (CauA.UI.Pressed){
                                CauA.UI.ButtonState = 0;
                                CauA.UI.Pressed = false;
                                CauA.Choosing = false;
                            } else if (CauB.UI.Pressed){
                                CauB.UI.ButtonState = 0;
                                CauB.UI.Pressed = false;
                                CauB.Choosing = false;
                            } else if (CauC.UI.Pressed){
                                CauC.UI.ButtonState = 0;
                                CauC.UI.Pressed = false;
                                CauC.Choosing = false;
                            }
                            CauD.UI.ButtonState = 1;
                            CauD.Choosing = true;
                            CauD.UI.Pressed = true;
                        }
                    }
                }
            }
        }
        SDL_RenderClear(renderer); // Xóa màn hình
        if (std::strcmp(where, "menu") == 0){
            SDL_RenderCopy(renderer, MenuBackground, nullptr, &MenuBackgroundRenderArea);

            SDL_RenderCopy(renderer, All_Menu_Button, &Play.Button[Play.ButtonState], &Play.ButtonRenderArea);
            SDL_RenderCopy(renderer, All_Menu_Button, &Exit.Button[Exit.ButtonState], &Exit.ButtonRenderArea);

            if (Play.Pressed){
                SDL_RenderPresent(renderer); // Render nút ở trạng thái nhấn vào
                SDL_Delay(100); // Delay 100 ms để thấy rõ sự thay đổi khi nhấn vào nút Play
                
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, All_Menu_Button, &Play.Button[1], &Play.ButtonRenderArea);
                SDL_RenderCopy(renderer, All_Menu_Button, &Exit.Button[Exit.ButtonState], &Exit.ButtonRenderArea);

                SDL_RenderPresent(renderer);

                std::strcpy(where, "game"); // Vào chế độ game
                std::cout << "Chào mừng đến với bình nguyên vô tận." << std::endl;

                ChangeCursor(normal);
                Play.Pressed = false;
                continue;
            } else if (Exit.Pressed){
                SDL_RenderPresent(renderer); // Render nút ở trạng thái nhấn vào
                SDL_Delay(100); // Delay 100 ms để thấy rõ sự thay đổi khi nhấn vào nút thoát
                
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, All_Menu_Button, &Play.Button[Play.ButtonState], &Play.ButtonRenderArea);
                SDL_RenderCopy(renderer, All_Menu_Button, &Exit.Button[1], &Exit.ButtonRenderArea);

                SDL_RenderPresent(renderer);
                Exit.Pressed = false;
                std::cout << "Đã nhấn nút thoát, đang dọn dẹp tài nguyên." << std::endl;
                run = false;
            }
        }
        else if (std::strcmp(where, "game") == 0){
            if (end){
                SDL_RenderClear(renderer);
                SDL_RenderPresent(renderer);
                std::cout << "Chúc mừng bạn đã trả lời hết tất cả câu hỏi !" << std::endl;
                MessageBoxW(NULL, L"Chúc mừng bạn, bạn đã trả lời hết tất cả câu hỏi !", L"Thông báo", MB_OK | MB_ICONINFORMATION);
                std::cout << "Game kết thúc. Đang dọn dẹp bộ nhớ." << std::endl;
                run = false;
                break;
            } if (health.HealthBarState == 0) {
                SDL_RenderClear(renderer);
                SDL_RenderPresent(renderer);
                std::cout << "Bạn đã thua do trả lời sai hoặc không trả lời quá 5 lần. Cố gắng lần sau nhé !" << std::endl;
                MessageBoxW(NULL, L"Bạn thua vì đã trả lời sai hoặc không trả lời quá 5 lần ! Cố gắng lần sau bạn nhé !", L"Bạn đã thua !", MB_OK | MB_ICONINFORMATION);
                std::cout << "Game kết thúc. Đang dọn dẹp bộ nhớ." << std::endl;
                run = false;
                break;
            }
            // Render hình nền
            SDL_RenderCopy(renderer, GameBackground, nullptr, &GameBackgroundRenderArea);
            // Render bộ đếm
            SDL_RenderCopy(renderer, AnsCounter.texture, nullptr, &AnsCounter.TextRenderArea);
            SDL_RenderCopy(renderer, TrueCounter.texture, nullptr, &TrueCounter.TextRenderArea);
            
            // Nếu texture của câu hỏi thay đổi tức là câu hỏi được thay đổi, phải tạo lại texture cho các câu trả lời
            if (UpdateQuestionTexture(renderer, question, font, w, h) == 1) {
                // Lấy câu trả lời cho câu hỏi mới
                AnsDataVariant = questions[question.question];
                AnsDataMap = ConvertVariantToMap(AnsDataVariant);
                for (auto& pair : AnsDataMap){
                    if (pair.first == "A") CauA.cautraloi = ConvertVariantToString(pair.second);

                    if (pair.first == "B") CauB.cautraloi = ConvertVariantToString(pair.second);

                    if (pair.first == "C") CauC.cautraloi = ConvertVariantToString(pair.second);

                    if (pair.first == "D") CauD.cautraloi = ConvertVariantToString(pair.second);
                }
                UpdateAnswerTexture(renderer, CauA, font, h);
                UpdateAnswerTexture(renderer, CauB, font, h);
                UpdateAnswerTexture(renderer, CauC, font, h);
                UpdateAnswerTexture(renderer, CauD, font, h);

                // Đồng thời cập nhật lại vị trí y của nút
                CauA.UI.ButtonRenderArea.y = question.RenderArea.y + CauA.UI.ButtonRenderArea.h / 2 + 50;
                CauA.RenderArea.y = CauA.UI.ButtonRenderArea.y + CauA.UI.ButtonRenderArea.h / 2 - CauA.RenderArea.h / 2; // Căn giữa nút

                CauB.UI.ButtonRenderArea.y = CauA.UI.ButtonRenderArea.y + CauA.UI.ButtonRenderArea.h + 5;
                CauB.RenderArea.y = CauB.UI.ButtonRenderArea.y + CauB.UI.ButtonRenderArea.h / 2 - CauB.RenderArea.h / 2;

                CauC.UI.ButtonRenderArea.y = CauB.UI.ButtonRenderArea.y + CauB.UI.ButtonRenderArea.h + 5;
                CauC.RenderArea.y = CauC.UI.ButtonRenderArea.y + CauC.UI.ButtonRenderArea.h / 2 - CauC.RenderArea.h / 2;

                CauD.UI.ButtonRenderArea.y = CauC.UI.ButtonRenderArea.y + CauC.UI.ButtonRenderArea.h + 5;
                CauD.RenderArea.y = CauD.UI.ButtonRenderArea.y + CauD.UI.ButtonRenderArea.h / 2 - CauD.RenderArea.h / 2;

                Update_CreateTexture(renderer, TrueCounter, font);
            }
            // Render câu hỏi
            SDL_RenderCopy(renderer, question.texture, nullptr, &question.RenderArea);

            // Render nút
            // RenderCopy trước câu hỏi để cái nút ở phía dưới cái chữ
            SDL_RenderCopy(renderer, CauA.UITexture, &CauA.UI.Button[CauA.UI.ButtonState], &CauA.UI.ButtonRenderArea);
            SDL_RenderCopy(renderer, CauB.UITexture, &CauB.UI.Button[CauB.UI.ButtonState], &CauB.UI.ButtonRenderArea);
            SDL_RenderCopy(renderer, CauC.UITexture, &CauC.UI.Button[CauC.UI.ButtonState], &CauC.UI.ButtonRenderArea);
            SDL_RenderCopy(renderer, CauD.UITexture, &CauD.UI.Button[CauD.UI.ButtonState], &CauD.UI.ButtonRenderArea);

            // Render chữ
            SDL_RenderCopy(renderer, CauA.texture, nullptr, &CauA.RenderArea);
            SDL_RenderCopy(renderer, CauB.texture, nullptr, &CauB.RenderArea);
            SDL_RenderCopy(renderer, CauC.texture, nullptr, &CauC.RenderArea);
            SDL_RenderCopy(renderer, CauD.texture, nullptr, &CauD.RenderArea);
            
            // Render nút Ok
            SDL_RenderCopy(renderer, Submit_Button_Texture, &Submit.Button[Submit.ButtonState], &Submit.ButtonRenderArea);

            // Render thanh máu
            SDL_RenderCopy(renderer, health.texture, &health.Button[health.HealthBarState], &health.RenderArea);
            if (Submit.Pressed){
                SDL_RenderPresent(renderer);
                SDL_Delay(100); // Áp dụng cơ chế như 2 nút ở Menu

                SDL_RenderClear(renderer);
                
                // Render hình nền
                SDL_RenderCopy(renderer, GameBackground, nullptr, &GameBackgroundRenderArea);
                // Render bộ đếm
                SDL_RenderCopy(renderer, AnsCounter.texture, nullptr, &AnsCounter.TextRenderArea);
                SDL_RenderCopy(renderer, TrueCounter.texture, nullptr, &TrueCounter.TextRenderArea);
                // Render câu hỏi
                SDL_RenderCopy(renderer, question.texture, nullptr, &question.RenderArea);
                // Render nút
                SDL_RenderCopy(renderer, CauA.UITexture, &CauA.UI.Button[CauA.UI.ButtonState], &CauA.UI.ButtonRenderArea);
                SDL_RenderCopy(renderer, CauB.UITexture, &CauB.UI.Button[CauB.UI.ButtonState], &CauB.UI.ButtonRenderArea);
                SDL_RenderCopy(renderer, CauC.UITexture, &CauC.UI.Button[CauC.UI.ButtonState], &CauC.UI.ButtonRenderArea);
                SDL_RenderCopy(renderer, CauD.UITexture, &CauD.UI.Button[CauD.UI.ButtonState], &CauD.UI.ButtonRenderArea);

                // Render chữ
                SDL_RenderCopy(renderer, CauA.texture, nullptr, &CauA.RenderArea);
                SDL_RenderCopy(renderer, CauB.texture, nullptr, &CauB.RenderArea);
                SDL_RenderCopy(renderer, CauC.texture, nullptr, &CauC.RenderArea);
                SDL_RenderCopy(renderer, CauD.texture, nullptr, &CauD.RenderArea);
                
                // Render nút Ok
                SDL_RenderCopy(renderer, Submit_Button_Texture, &Submit.Button[0], &Submit.ButtonRenderArea);

                // Render thanh máu
                SDL_RenderCopy(renderer, health.texture, &health.Button[health.HealthBarState], &health.RenderArea);

                SDL_RenderPresent(renderer);
                Submit.Pressed = false;
                
                if (CauA.Choosing) {
                    std::cout << "Câu trả lời: Câu A. ";
                    if (IsAnswerTrue(questions, question.question, "A")){
                        std::cout << "Đúng" << std::endl;
                        AnswerTrue = true;
                    } else {
                        std::cout << "Sai" << std::endl;
                        AnswerTrue = false;
                    }
                } else if (CauB.Choosing) {
                    std::cout << "Câu trả lời: Câu B. ";
                    if (IsAnswerTrue(questions, question.question, "B")){
                        std::cout << "Đúng" << std::endl;
                        AnswerTrue = true;
                    } else {
                        std::cout << "Sai" << std::endl;
                        AnswerTrue = false;
                    }
                } else if (CauC.Choosing) {
                    std::cout << "Câu trả lời: Câu C. ";
                    if (IsAnswerTrue(questions, question.question, "C")){
                        std::cout << "Đúng" << std::endl;
                        AnswerTrue = true;
                    } else {
                        std::cout << "Sai" << std::endl;
                        AnswerTrue = false;
                    }
                } else if (CauD.Choosing) {
                    std::cout << "Câu trả lời: Câu D. ";
                    if (IsAnswerTrue(questions, question.question, "D")){
                        std::cout << "Đúng" << std::endl;
                        AnswerTrue = true;
                    } else {
                        std::cout << "Sai" << std::endl;
                        AnswerTrue = false;
                    }
                } else if (!CauA.Choosing && !CauB.Choosing && !CauC.Choosing && !CauD.Choosing){
                    std::cout << "Không chọn câu trả lời" << std::endl;
                    Mix_PlayChannel(-1, IncorrectSoundEffect, 0);
                    std::cout << "Đang phát sound effect 'incorrect'." << std::endl;
                    AnswerTrue = false;
                }
                // Xóa câu hỏi đã trả lời
                questions.erase(question.question);
                // Random lại câu hỏi
                if (questions.size() != 0) {
                    question.question = RandomQuestion(questions);
                    question.NeedToChange = true;
                } else end = true;
                CauA.Choosing = false;
                CauB.Choosing = false;
                CauC.Choosing = false;
                CauD.Choosing = false;

                CauA.UI.ButtonState = 0;
                CauB.UI.ButtonState = 0;
                CauC.UI.ButtonState = 0;
                CauD.UI.ButtonState = 0;

                if (AnswerTrue)
                {
                    TrueAns++;
                    Mix_PlayChannel(-1, CorrectSoundEffect, 0);
                    std::cout << "Đang phát 'correct.wav'." << std::endl;

                    while (TrueAns == 5 && av_read_frame(formatCtx, &packet) >= 0){
                        SDL_Window* Video_Window;
                        const char* name_of_window_ = "Mời bạn xem đoạn video này";
                        if (StartWindow(Video_Window, name_of_window_, VideoW / 2, VideoH / 2) == 1){
                            SDL_DestroyTexture(All_Menu_Button);
                            SDL_DestroyTexture(Submit_Button_Texture);
                            SDL_DestroyTexture(MenuBackground);
                            SDL_DestroyTexture(GameBackground);
                            SDL_DestroyTexture(CauA.UITexture);
                            SDL_DestroyTexture(CauB.UITexture);
                            SDL_DestroyTexture(CauC.UITexture);
                            SDL_DestroyTexture(CauD.UITexture);
                            SDL_DestroyTexture(health.texture);
                            Mix_FreeChunk(CorrectSoundEffect);
                            Mix_FreeChunk(IncorrectSoundEffect);
                            SDL_DestroyRenderer(renderer);
                            SDL_DestroyWindow(win);
                            QuitFont();
                            QuitImage();
                            SoundCleanup();
                            SDL_Quit();
                        }
                        if (packet.stream_index == videoStream) {
                            avcodec_send_packet(codecCtx, &packet);
                            while (avcodec_receive_frame(codecCtx, frame) == 0) {

                                sws_scale(swsCtx, frame->data, frame->linesize, 0, VideoH, yuvFrame->data, yuvFrame->linesize);

                                SDL_UpdateYUVTexture(VideoTexture, nullptr, yuvFrame->data[0], yuvFrame->linesize[0], yuvFrame->data[1], yuvFrame->linesize[1], yuvFrame->data[2], yuvFrame->linesize[2]);

                                SDL_RenderClear(renderer);
                                SDL_RenderCopy(renderer, VideoTexture, nullptr, nullptr);
                                SDL_RenderPresent(renderer);

                                SDL_Delay(16); // ~30fps
                            }
                        }

                        av_packet_unref(&packet);

                        while (SDL_PollEvent(&event)) {
                            if (event.type == SDL_QUIT)
                                run = false;
                        }
                    }
                    if (TrueAns == 5) {round++; TrueAns = 0;}
                }

                if (!AnswerTrue)
                {
                    Mix_PlayChannel(-1, IncorrectSoundEffect, 0);
                    std::cout << "Đang phát 'incorrect.wav'." << std::endl;
                    health.HealthBarState--;
                }
            }
        }
        SDL_RenderPresent(renderer); // Render
    }
    SDL_FreeCursor(normal);
    SDL_FreeCursor(clickable);
    SDL_DestroyTexture(All_Menu_Button);
    SDL_DestroyTexture(Submit_Button_Texture);
    SDL_DestroyTexture(MenuBackground);
    SDL_DestroyTexture(GameBackground);
    SDL_DestroyTexture(CauA.UITexture);
    SDL_DestroyTexture(CauB.UITexture);
    SDL_DestroyTexture(CauC.UITexture);
    SDL_DestroyTexture(CauD.UITexture);
    SDL_DestroyTexture(CauA.texture);
    SDL_DestroyTexture(CauB.texture);
    SDL_DestroyTexture(CauC.texture);
    SDL_DestroyTexture(CauD.texture);
    SDL_DestroyTexture(health.texture);
    Mix_FreeChunk(CorrectSoundEffect);
    Mix_FreeChunk(IncorrectSoundEffect);
    Mix_FreeChunk(HitSoundEffect);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    TTF_CloseFont(font);
    QuitFont();
    QuitImage();
    SoundCleanup();
    SDL_Quit();
    return 0;
}