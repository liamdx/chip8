#include <SDL.h>
#include <vector>
#include <stdlib.h>
#include <cmath>
#include <functional>

#undef main

#define SDL_WHITE 255, 255, 255, 255
#define SDL_BLACK 0,0,0,0

constexpr uint16_t SCALE_FACTOR = 64;

uint32_t SDL_FLAGS;
SDL_Window* m_Window;
SDL_Renderer* m_Renderer;
SDL_Rect* m_Viewport;
std::vector<SDL_Keycode> m_ActiveKeycodes;

bool m_ActiveKeys[16] = { false };

bool m_ShouldRun = true;

uint16_t real_screen_x = 0;
uint16_t real_screen_y = 0;

int GetKeycodeArrayIndex(SDL_Keycode keycode)
{
    for (int i = 0; i < 16; i++)
    {
        if (m_ActiveKeycodes[i] == keycode)
        {
            return i;
        }
    }
    return -1;
}

void HandleKeydown(SDL_Event& event)
{
    int index = GetKeycodeArrayIndex(event.key.keysym.sym);

    if (index > -1)
    {
        m_ActiveKeys[index] = true;
    }
}

void HandleKeyup(SDL_Event& event)
{
    int index = GetKeycodeArrayIndex(event.key.keysym.sym);

    if (index > -1)
    {
        m_ActiveKeys[index] = false;
    }
}

void HandleSDLEvent(SDL_Event& event)
{
    switch (event.type)
    {
        case SDL_QUIT:
            m_ShouldRun = false;
            break;
        case SDL_KEYDOWN:
            HandleKeydown(event);
            break;
        case SDL_KEYUP:
            HandleKeyup(event);
            break;
    }
}

void CreateSDLWindow(uint16_t width, uint16_t height)
{
    SDL_FLAGS |= SDL_WINDOW_ALLOW_HIGHDPI;
    SDL_FLAGS |= SDL_WINDOW_RESIZABLE;
    real_screen_x = width * std::sqrt(SCALE_FACTOR);
    real_screen_y = height * std::sqrt(SCALE_FACTOR);
    m_Window = SDL_CreateWindow("Chip8 Emu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, real_screen_x, real_screen_y, SDL_FLAGS);
    if (m_Window == NULL)
    {
        std::cout << "Failed to Create SDL Window: " << SDL_GetError() << std::endl;
    }
    m_Viewport = new SDL_Rect();
    m_Viewport->w = real_screen_x;
    m_Viewport->h = real_screen_y;
}

void CreateSDLRenderer()
{
    m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_SOFTWARE);
    if (m_Renderer == NULL)
    {
        std::cout << "Failed to Create SDL Renderer: " << SDL_GetError() << std::endl;
    }
}

void SetupInput()
{
    m_ActiveKeycodes.emplace_back(49);
    m_ActiveKeycodes.emplace_back(50);
    m_ActiveKeycodes.emplace_back(51);
    m_ActiveKeycodes.emplace_back(52);
    m_ActiveKeycodes.emplace_back(113);
    m_ActiveKeycodes.emplace_back(119);
    m_ActiveKeycodes.emplace_back(101);
    m_ActiveKeycodes.emplace_back(114);
    m_ActiveKeycodes.emplace_back(97);
    m_ActiveKeycodes.emplace_back(115);
    m_ActiveKeycodes.emplace_back(100);
    m_ActiveKeycodes.emplace_back(102);
    m_ActiveKeycodes.emplace_back(122);
    m_ActiveKeycodes.emplace_back(120);
    m_ActiveKeycodes.emplace_back(99);
    m_ActiveKeycodes.emplace_back(118);
}

void InitSDL(uint16_t w, uint16_t h)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Failed to init SDL: " << SDL_GetError() << std::endl;
    }
    CreateSDLWindow(w,h);
    CreateSDLRenderer();
    SetupInput();
}

void RunLoop(std::function<void()> callback)
{
    while (m_ShouldRun)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            HandleSDLEvent(event);
        }
        callback();
    }
}

uint16_t GetKeyboardState()
{
    uint16_t keyboard_state = 0x0000;

    for (int i = 0; i < 16; i++)
    {
        keyboard_state |= m_ActiveKeys[i] << (15 - i);
    }

    return keyboard_state;
}

void DrawBoolArray(std::vector<bool> pixels, uint16_t width, uint16_t height)
{
    SDL_SetRenderDrawColor(m_Renderer, SDL_BLACK);
    SDL_RenderSetViewport(m_Renderer, m_Viewport);
    SDL_RenderClear(m_Renderer);
    SDL_Color current_colour = SDL_Color();

    for (uint16_t w = 0; w < width; w++)
    {
        for (uint16_t h = 0; h < height; h++)
        {
            uint16_t index = ((w + 1) * (h + 1)) - 1;
            if (pixels[index])
            {
                current_colour.r = current_colour.g = current_colour.b = (Uint8)255;
            }
            else
            {
                current_colour.r = current_colour.g = current_colour.b = (Uint8)0;
            }
            SDL_SetRenderDrawColor(m_Renderer, current_colour.r, current_colour.g, current_colour.b, 255);
            SDL_RenderDrawPoint(m_Renderer, w, h);
        }
    }
    
    SDL_RenderPresent(m_Renderer);
}

void DrawBoolArrayScaled(std::vector<bool> pixels, uint16_t width, uint16_t height)
{
    SDL_SetRenderDrawColor(m_Renderer, SDL_BLACK);
    SDL_RenderSetViewport(m_Renderer, m_Viewport);
    SDL_RenderClear(m_Renderer);
    SDL_Color current_colour = SDL_Color();
    uint16_t pixels_per_axis = std::sqrt(SCALE_FACTOR);

    int numPixelsDrawn = 0;

    for (uint16_t h = 0; h < height; h++)
    {
        for (uint16_t w = 0; w < width; w++)
        {
            uint16_t index = w + (h * width);
            if (pixels[index])
            {
                current_colour.r = current_colour.g = current_colour.b = (Uint8)255;
            }
            else
            {
                current_colour.r = current_colour.g = current_colour.b = (Uint8)0;
            }

            // e.g. scale factor is 16
            // additional pixels in each axis is std::sqrt(SCALE_FACTOR)
            for (uint16_t rw = 0; rw < pixels_per_axis; rw++)
            {
                for (uint16_t rh = 0; rh < pixels_per_axis; rh++)
                {
                    SDL_SetRenderDrawColor(m_Renderer, current_colour.r, current_colour.g, current_colour.b, 255);
                    SDL_RenderDrawPoint(m_Renderer, (pixels_per_axis * w) + rw, (pixels_per_axis * h) + rh);
                    numPixelsDrawn++;
                }
            }            
        }
    }
    SDL_RenderPresent(m_Renderer);
}

std::vector<bool> CreateTestBoolArray(uint16_t width, uint16_t height)
{
    std::vector<bool> bools;
    for (int i = 0; i < width * height; i++)
    {
        if (i % 2 == 0)
        {
            bools.emplace_back(true);
        }
        else
        {
            bools.emplace_back(false);
        }
    }
    return bools;
}
