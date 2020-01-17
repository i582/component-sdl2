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
}