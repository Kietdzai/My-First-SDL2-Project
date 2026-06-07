# SDL2 Project
Trò chơi giải đố, chủ đề "Làng nghề truyền thống ở địa phương em", Làng Bánh khô mè Bà Liễu Mẹ

Phân chia công việc:
    Nguyễn Tuấn Kiệt: Lập trình, tìm ảnh, tìm hiệu ứng âm thanh
    Nguyễn Văn Phú: Tìm ảnh, soạn câu hỏi, tìm nhạc nền

Nguồn tài nguyên:
    Answer.png: từ https://vio.edu.vn
    TickIcon.png: từ https://www.flaticon.com/free-icon/check_14090371?term=tick&page=1&position=1&origin=tag&related_id=14090371
    FailIcon.png: từ https://www.flaticon.com/free-icon/remove_1828843?term=fail&page=1&position=2&origin=search&related_id=1828843
    SubmitButton.png: từ https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcR_GvsXf4Z5uBD-U3uQqbPieCyXrKgfFiqgJw&s
    MenuButton.png: từ https://github.com/FakeMG/My_First_SDL_Game/blob/main/res/texture/Button.png
    TimesNewRoman.ttf: không rõ
    GameBackground.png: https://laodong.vn/du-lich/am-thuc/lang-nghe-kho-me-da-nang-dac-san-banh-qua-noi-tieng-viet-nam-1137869.html
                Chỉnh sửa + Thêm giao diện người dùng: Photoshop 2024
    MenuBackground.png: từ Nguyễn Văn Phú (tìm và gửi)
    incorrect.wav: từ https://pixabay.com/sound-effects/search/correct/ (dưới dạng mp3)
                   công cụ chuyển đổi: https://mp3cut.net/vi/
    correct.wav: từ https://www.myinstants.com/en/instant/ding-sound-effect/ (dưới dạng mp3)
                   công cụ chuyển đổi: https://mp3cut.net/vi/

Nguồn soạn câu hỏi (theo thứ tự trong tệp tin questions.json ở đường dẫn assets/questions):
    Lưu ý: Tất cả các giá trị (trừ giá trị của "Answer") đều đã được chuyển đổi sang mã Unicode (dạng Escape).
        Nguồn chuyển đổi: https://dencode.com/string/unicode-escape
        Mục đích là để tránh gian lận khi vừa trả lời câu hỏi vừa mở tệp tin 'questions.json'
        
    Câu 1, 2: từ https://laodong.vn/du-lich/am-thuc/lang-nghe-kho-me-da-nang-dac-san-banh-qua-noi-tieng-viet-nam-1137869.html
    Câu 3, 8: từ Nguyễn Văn Phú

Phiên bản MinGW64:
    MinGW64-W64 64-bit 15.2.0
        từ: https://winlibs.com/

Phiên bản của các thư viện SDL (dựng sẵn trong thư viện và ở ngoài):
    SDL (dựng sẵn): 2.32.10
        từ: https://github.com/libsdl-org/SDL/releases/tag/release-2.32.10
        tệp tin: SDL2-devel-2.32.10-mingw.zip
    SDL_image (ở ngoài): 2.8.8
        từ: https://github.com/libsdl-org/SDL_image/releases/tag/release-2.8.8
        tệp tin: SDL2_image-devel-2.8.8-mingw.zip
    SDL_ttf (ở ngoài): 2.24.0
        từ: https://github.com/libsdl-org/SDL_ttf/releases/tag/release-2.24.0
        tệp tin: SDL2_ttf-devel-2.24.0-mingw.zip
    SDL_mixer (ở ngoài): 2.8.1
        từ: https://github.com/libsdl-org/SDL_mixer/release-2.8.1
        tệp tin: SDL2_mixer-devel-2.8.1-mingw.zip

Cách chơi:
    Đầu tiên, nhấn đúp chuột vào app.exe. Nếu có lỗi, có lẽ môi trường chạy không phù hợp. Hãy chắc chắn rằng trong thư mục đang chứa 'app.exe' có các tệp tin SDL2.dll, SDL_image.dll, SDL_ttf.dll và SDL_mixer.dll.

    Tiếp theo, để bắt đầu chơi, nhấn vào nút hình tam giác (nút Play) để vào trò chơi.
               để thoát ra, nhấn vào nút còn lại.
    
    Sau đó, sẽ có câu hỏi hiện ra, nhấn chọn câu trả lời và nhấn vào nút OK.

    Nếu đúng, nhân vật ở trên sẽ bắn trúng mục tiêu, nếu không, sẽ không trúng mục tiêu. Nếu quá 5 lần sai thì nhân vật sẽ bị đánh bại.

Lưu ý:
    Tệp tin JSONObject.cpp và JSONObject.h đã được làm cho mục đích cá nhân của người làm ra nó, đã được ứng dụng vào trong trò chơi này. Tệp tin này hoàn toàn miễn phí, tất cả mọi người đều có thể chỉnh sửa bên trong nó. Bản gốc: https://github.com/Kietdzai/My-First-JSON-Object-Parser

    `.h` tức là tệp tin dùng để khai báo, là cầu nối giữa tệp tin `.cpp` của nó và tệp tin `main.cpp`
    `.cpp` là tệp tin dùng để viết hàm thực hiện khi được gọi ở `main.cpp`

    Tệp tin `Makefile` dùng để xây dựng dự án này sang `.exe` - tệp tin mà Windows có thể chạy và thực thi. Chạy bằng cách gõ `mingw32-make` vào CMD hay Windows PowerShell

    Nếu muốn biên dịch lại phần mềm, hãy đặt lại các thư mục của thư viện C++, của thư viện SDL như trong tệp tin `c_cpp_properties.json` ở trong thư mục `.vscode`