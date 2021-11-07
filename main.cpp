#include <iostream>
#include <functional>
#include "sdl_helpers.hpp"
#include "chip8.h"



int main()
{
    std::cout << "Hello Chip8" << std::endl;
    InitSDL(SCREEN_WIDTH , SCREEN_HEIGHT);
    chip8 emu;

    auto bools = CreateTestBoolArray(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    RunLoop([&]()
    {
        // DrawBoolArray(bools, SCREEN_WIDTH, SCREEN_HEIGHT);
        DrawBoolArrayScaled(bools, SCREEN_WIDTH, SCREEN_HEIGHT);
    });

    return 1;
}