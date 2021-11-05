#include <iostream>
#include <SDL.h>
#undef main
uint32_t SDL_FLAGS;
SDL_Window* m_Window;
bool m_ShouldRun = true;

void HandleSDLEvent(SDL_Event& event)
{
    switch (event.type)
    {
    case SDL_QUIT:
        m_ShouldRun = false;
        break;
    }
}

int main()
{
    std::cout << "Hello Chip8" << std::endl;
    SDL_FLAGS |= SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
    m_Window = SDL_CreateWindow("Chip8 Emu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 600, SDL_FLAGS);

    while (m_ShouldRun)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            HandleSDLEvent(event);
        }
    }
    return 1;
}