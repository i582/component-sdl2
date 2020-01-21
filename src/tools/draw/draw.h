#pragma once

#include "SDL.h"
#include "SDL2_gfxPrimitives.h"

#include "../rect/simple-rect/simple-rect.h"
#include "../css/color/color.h"
#include "../utils/utils.h"


namespace Kit::Draw
{
    using CSS::Color;

    void roundedRect(SDL_Renderer* renderer, const SimpleRect& rect, const int& radius, const Color& color);

    void roundedRect(SDL_Renderer* renderer, const SimpleRect& rect,
            const int& radius_1, const int& radius_2, const int& radius_3, const int& radius_4, const Color& color);
}