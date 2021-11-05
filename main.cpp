#include <iostream>
#include <functional>
#include "sdl_helpers.hpp"
#include "chip8.h"



int main()
{
    std::cout << "Hello Chip8" << std::endl;

    chip8 emu;

    CreateSDLWindow(SCREEN_WIDTH * 16, SCREEN_HEIGHT * 16);
    RunLoop([&]()
    {
        std::cout << "Hello again!" << std::endl;
    });

    return 1;
}