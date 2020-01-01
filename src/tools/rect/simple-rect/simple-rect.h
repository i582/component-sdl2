#pragma once

#include "SDL.h"

namespace Kit
{
	using SimpleRect = SDL_Rect;

	namespace SimpleRectFunc
    {
        bool equal(const SimpleRect& first, const SimpleRect& second);
    }

}