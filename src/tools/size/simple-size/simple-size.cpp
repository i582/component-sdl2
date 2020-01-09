#include "simple-size.h"

Kit::SimpleSize::SimpleSize() : SimpleSize(0, 0) {}

Kit::SimpleSize::SimpleSize(int w, int h)
{
    this->w = w;
    this->h = h;
}

SDL_Point Kit::SimpleSize::toSdlPoint()
{
    return {w, h};
}

