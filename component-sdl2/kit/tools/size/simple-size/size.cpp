#include "size.h"

Lib::SimpleSize::SimpleSize() : SimpleSize(0, 0) {}

Lib::SimpleSize::SimpleSize(int w, int h)
{
	this->w = w;
	this->h = h;
}

SDL_Point Lib::SimpleSize::toSdlPoint()
{
	return { w, h };
}

