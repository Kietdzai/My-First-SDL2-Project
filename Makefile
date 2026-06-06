all:
	g++ main.cpp Window.cpp Questions.cpp Cursor.cpp LoadImage.cpp Game.cpp Answer.cpp Font.cpp JSONObject.cpp Button.cpp Counter.cpp Sound.cpp HealthBar.cpp Video.cpp \
	-IC:/ffmpeg/include -LC:/ffmpeg/lib -lavformat -lavcodec -lavutil -lswscale -lws2_32 -lsecur32 -lbcrypt \
	-lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -o app.exe -std=c++17