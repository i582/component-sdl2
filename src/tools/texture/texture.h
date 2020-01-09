#pragma once

#include "SDL.h"

#include "extended-rect.h"
#include "simple-rect.h"
#include "simple-size.h"

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