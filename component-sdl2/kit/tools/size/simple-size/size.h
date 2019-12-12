#pragma once

#include "SDL.h"

namespace Lib
{
	struct SimpleSize
	{
		int w, h;

		SimpleSize();
		SimpleSize(int w, int h);

		SDL_Point toSdlPoint();
	};
}