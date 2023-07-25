#include "../include/gpu.hpp"
#include <iostream>

Gpu::Gpu(Buses *b, int width, int height):
Peripheral::Peripheral(END_ADDRESS - (width * height * 4), END_ADDRESS)
{
    this->buses = b;
    this->width = width;
    this->height = height;
    this->frameBuffer = new uint8[width * height * 4];

    for (long int i = 0; i < (width * height * 4); i++)
        this->frameBuffer[i] = 0;

    if(SDL_Init(SDL_INIT_VIDEO) != 0) // SDL_INIT_AUDIO
    {
        std::cerr << SDL_GetError() << std::endl;
        return;
    }

    // create window
    this->window = SDL_CreateWindow("vm",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        this->width * 2,
        this->height * 2, 0);
    if (this->window == NULL) {
        SDL_Log("Unable to create window: %s", SDL_GetError());
        return;
    }

    // create renderer
    this->renderer = SDL_CreateRenderer(
        this->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (this->renderer == NULL) {
        SDL_Log("Unable to create renderer: %s", SDL_GetError());
        return;
    }
    SDL_RenderSetLogicalSize(renderer, this->width, this->height);

    // create texture
    this->texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        this->width,
        this->height);
    if (texture == NULL) {
        SDL_Log("Unable to create texture: %s", SDL_GetError());
        return;
    }

    this->updateTexture();
}

Gpu::~Gpu()
{
    delete this->frameBuffer;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

std::string Gpu::getName()
{
    return "RAM, from " + std::to_string(this->beginning_address) + " to " + std::to_string(this->end_address);
}

void Gpu::execOrder()
{
    if (this->buses->getReadWritePin() == READ)
    {
        this->buses->setDataBus(this->frameBuffer[END_ADDRESS - this->buses->getAddressBus()]);
    }
    else if (this->buses->getReadWritePin() == WRITE)
    {
        this->frameBuffer[END_ADDRESS - this->buses->getAddressBus()] = (uint8)(this->buses->getDataBus() & 0xFF);
    }
}


void Gpu::updateTexture()
{
    int texture_pitch = 0;
    void* texture_pixels = NULL;
    if (SDL_LockTexture(this->texture, NULL, &texture_pixels, &texture_pitch) != 0) {
        SDL_Log("Unable to lock texture: %s", SDL_GetError());
    }
    else {
        memcpy(texture_pixels, this->frameBuffer, texture_pitch * this->height);
    }
    SDL_UnlockTexture(texture);
}

void Gpu::displayFrame()
{
    // render on screen
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}