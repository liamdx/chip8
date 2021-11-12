#include <iostream>
#include <functional>
#include "sdl_helpers.hpp"
#include "chip8.h"

int main()
{
    std::cout << "Hello Chip8" << std::endl;
    InitSDL(SCREEN_WIDTH , SCREEN_HEIGHT);
    chip8 emu;
    // std::vector<uint8_t> rom = emu.LoadRomStream("res/roms/ibm_test.ch8");
    std::vector<uint8_t> rom = emu.LoadRomStream("res/roms/pong.ch8");
    emu.LoadRom(rom);

    RunLoop([&]()
    {
        emu.Update(GetKeyboardState());
        auto pixels = emu.GetPixels();
        DrawBoolArrayScaled(pixels, SCREEN_WIDTH, SCREEN_HEIGHT);
    });

    return 1;
}