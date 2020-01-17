#pragma once

#include "SDL.h"

#include "../rect/extended-rect/extended-rect.h"
#include "../rect/simple-rect/simple-rect.h"
#include "../size/simple-size/simple-size.h"

namespace Kit
{

    class Texture
    {
    public:
        static SDL_Texture* create(SDL_Renderer* renderer, const Rect& size);
        static SDL_Texture* create(SDL_Renderer* renderer, const Size& size);
        static SDL_Texture* create(SDL_Renderer* renderer, const SimpleRect& size);
        static SDL_Texture* create(SDL_Renderer* renderer, const SimpleSize& size);

        static void destroy(SDL_Texture* texture);
    };

}