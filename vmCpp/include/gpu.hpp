#ifndef GPU_HPP
#define GPU_HPP
#include "general.hpp"
#include "buses.hpp"
#include "peripheral.hpp"
#include <SDL2/SDL.h>
#define END_ADDRESS 0xFFFFFFFF

class Gpu: public Peripheral
{
    //640 x 360
    private:
        Buses *buses;
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Texture *texture;
        uint8 *frameBuffer;
        int width;
        int height;

    public:
        Gpu(Buses *b, int width, int height);
        virtual ~Gpu();
        std::string getName();
        void execOrder();
        void updateTexture();
        void displayFrame();


};

#endif