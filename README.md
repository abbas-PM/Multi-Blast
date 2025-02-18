# Multi-Blast
Simple game made in C++ and SDL2.

# ScreenShots
![image](https://github.com/user-attachments/assets/c16d58f5-13be-4deb-9f76-bf38eaeec2a0)
![image](https://github.com/user-attachments/assets/d1c14b08-ff22-408c-9c99-6d3d2cd438c2)

# Background 
Multi-Blast is a space shooter game, where you control a green and red ship, and fight aliens that are red or green. The green ship can only kill the green aliens and the red ship can only kill the red aliens. Controls:  A - Move Left, D - Move Right, Left - Green Ship shoots, Right - Red Ship shoots.

# Compiling 
To compile on windows, install Mingw64, SDL2, SDL2_Image, SDL2_TTF, and SDL2_Mixer. Place all the C header files from SDL2, SDL2_Image, SDL2_TTF, and SDL2_Mixer into the include folder, and create a new folder called lib and place all the library files, cmake files and pkgconfig files there. Finally create a bin folder and place the dll files there, thats also where the .exe file will be created. Run the line in the Makefile in the root directory to compile, then run the .exe file.
