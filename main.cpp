#include <iostream>
#include <functional>
#include "sdl_helpers.hpp"


int main()
{
    std::cout << "Hello Chip8" << std::endl;
    CreateSDLWindow();
    RunLoop([&]()
    {
        std::cout << "Hello again!" << std::endl;
    });

    return 1;
}