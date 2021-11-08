#include <SDL.h>
#include <vector>
#include <stdlib.h>
#undef main

#define SDL_WHITE 255, 255, 255, 255
#define SDL_BLACK 0,0,0,0

constexpr uint16_t SCALE_FACTOR = 121;

uint32_t SDL_FLAGS;
SDL_Window* m_Window;
SDL_Renderer* m_Renderer;
SDL_Rect* m_Viewport;
bool m_ShouldRun = true;

uint16_t real_screen_x = 0;
uint16_t real_screen_y = 0;


void HandleSDLEvent(SDL_Event& event)
{
    switch (event.type)
    {
    case SDL_QUIT:
        m_ShouldRun = false;
        break;
    }
}

void CreateSDLWindow(uint16_t width, uint16_t height)
{
    SDL_FLAGS |= SDL_WINDOW_ALLOW_HIGHDPI;
    real_screen_x = width * sqrt(SCALE_FACTOR);
    real_screen_y = height * sqrt(SCALE_FACTOR);
    m_Window = SDL_CreateWindow("Chip8 Emu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, real_screen_x, real_screen_y, SDL_FLAGS);
    m_Viewport = new SDL_Rect();
    m_Viewport->w = real_screen_x;
    m_Viewport->h = real_screen_y;
}

void CreateSDLRenderer()
{
    m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);
}

void InitSDL(uint16_t w, uint16_t h)
{
    CreateSDLWindow(w,h);
    CreateSDLRenderer();
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
    uint16_t pixels_per_axis = sqrt(SCALE_FACTOR);

    int numPixelsDrawn = 0;

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

            // e.g. scale factor is 16
            // additional pixels in each axis is sqrt(SCALE_FACTOR)
            for (uint16_t rw = 0; rw < pixels_per_axis; rw++)
            {
                for (uint16_t rh = 0; rh < pixels_per_axis; rh++)
                {
                    // 64x32 real res, 16 scale factor, 4 pixels per axis
                    // (0,0)  = x 0->3  y 0->3
                    // (1, 0) = x 4->7  y 0->3
                    // (2, 0) = x 8->11 y 0->3
                    // (2, 1) = x 8->11 y 4->7
                    // xstart = (pixels_per_axis * w)
                    // xend = xstart + rw
                    // ystart = pixels_per_axis * h
                    // yend = ystart + rh

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
