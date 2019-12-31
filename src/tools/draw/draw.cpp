#include "draw.h"

using namespace Kit;

void Draw::roundedRect(SDL_Renderer* renderer, const SimpleRect& rect, const int& radius, const Color& _color)
{
    const Uint32 color = _color.colorReverse() == 0 ? _color.colorReverse() : _color.colorReverse() - 1;
    const int x1 = rect.x;
    const int y1 = rect.y;
    const int x2 = rect.x + rect.w;
    const int y2 = rect.y + rect.h;

    const int cx1 = x1 + radius;
    const int cy1 = y1 + radius;

    const int cx2 = x2 - radius - 1;
    const int cy2 = y1 + radius;

    const int cx3 = x2 - radius - 1;
    const int cy3 = y2 - radius - 1;

    const int cx4 = x1 + radius;
    const int cy4 = y2 - radius - 1;

    const SDL_Rect r1 = { cx1, y1, rect.w - 2*radius, rect.h};
    const SDL_Rect r2 = { x1, cy1, rect.w, rect.h - 2*radius};

    if (radius == 0 || radius < 0)
    {
        SDL_SetRenderDrawColor(renderer, _color.r(), _color.g(), _color.b(), _color.a());
        SDL_RenderFillRect(renderer, &rect);
        return;
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);


    filledEllipseColor(renderer, cx1, cy1, radius, radius, color);
    aacircleColor(renderer, cx1, cy1, radius, color);

    filledEllipseColor(renderer, cx2, cy2, radius, radius, color);
    aacircleColor(renderer, cx2, cy2, radius, color);

    filledEllipseColor(renderer, cx3, cy3, radius, radius, color);
    aacircleColor(renderer, cx3, cy3, radius, color);

    filledEllipseColor(renderer, cx4, cy4, radius, radius, color);
    aacircleColor(renderer, cx4, cy4, radius, color);

    SDL_SetRenderDrawColor(renderer, _color.r(), _color.g(), _color.b(), _color.a());

    SDL_RenderFillRect(renderer, &r1);
    SDL_RenderFillRect(renderer, &r2);
}