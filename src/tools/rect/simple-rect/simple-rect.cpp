#include "simple-rect.h"

bool Kit::SimpleRectFunc::equal(const Kit::SimpleRect& first, const Kit::SimpleRect& second)
{
    return
            first.x == second.x &&
            first.y == second.y &&
            first.w == second.w &&
            first.h == second.h;
}
