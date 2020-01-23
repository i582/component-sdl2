#include "draw.h"

using namespace Kit;

void Draw::roundedRect(SDL_Renderer* renderer, const SimpleRect& rect, const int& radius, const Color& color)
{
    roundedRect(renderer, rect, radius, radius, radius, radius, color);
}

void Draw::roundedRect(SDL_Renderer* renderer, const SimpleRect& rect,
                       const int& radius_1, const int& radius_2, const int& radius_3, const int& radius_4, const Color& color_)
{
    const Uint32 color = color_.colorReverse() == 0 ? color_.colorReverse() : color_.colorReverse() - 1;
    const Sint16 x1 = rect.x;
    const Sint16 y1 = rect.y;
    const Sint16 x2 = rect.x + rect.w;
    const Sint16 y2 = rect.y + rect.h;

    const Sint16 cx1 = x1 + radius_1;
    const Sint16 cy1 = y1 + radius_1;

    const Sint16 cx2 = x2 - radius_2 - 1;
    const Sint16 cy2 = y1 + radius_2;

    const Sint16 cx3 = x2 - radius_3 - 1;
    const Sint16 cy3 = y2 - radius_3 - 1;

    const Sint16 cx4 = x1 + radius_4;
    const Sint16 cy4 = y2 - radius_4 - 1;

    const SDL_Rect r1 = {cx1, y1, rect.w - radius_1 - radius_2, rect.h};
    const SDL_Rect r2 = {x1, cy1, rect.w, rect.h - radius_3 - radius_4};


    Sint16 point_x[12] = { cx1, cx2, cx2, static_cast<Sint16>(x2 - 1), static_cast<Sint16>(x2 - 1), cx3, cx3, cx4, cx4, x1, x1, cx1 };
    Sint16 point_y[12] = { y1, y1, cy2, cy2, cy3, cy3, static_cast<Sint16>(y2 - 1), static_cast<Sint16>(y2 - 1), cy4, cy4, cy1, cy1 };




    if ((radius_1 == 0 || radius_1 < 0) &&
        (radius_2 == 0 || radius_2 < 0) &&
        (radius_3 == 0 || radius_3 < 0) &&
        (radius_3 == 0 || radius_3 < 0))
    {
        SDL_SetRenderDrawColor(renderer, color_.r(), color_.g(), color_.b(), color_.a());
        SDL_RenderFillRect(renderer, &rect);
        return;
    }

    if ((radius_1 == radius_2) && (radius_1 == radius_3) && (radius_1 == radius_4) && (radius_1 == rect.w/2) && (radius_1 == rect.h/2))
    {
        filledEllipseColor(renderer, cx1, cy1, radius_1 - 1, radius_1 - 1, color);
        aacircleColor(renderer, cx1, cy1, radius_1 - 1, color);
        return;
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);


    filledEllipseColor(renderer, cx1, cy1, radius_1, radius_1, color);
    aacircleColor(renderer, cx1, cy1, radius_1, color);

    filledEllipseColor(renderer, cx2, cy2, radius_2, radius_2, color);
    aacircleColor(renderer, cx2, cy2, radius_2, color);

    filledEllipseColor(renderer, cx3, cy3, radius_3, radius_3, color);
    aacircleColor(renderer, cx3, cy3, radius_3, color);

    filledEllipseColor(renderer, cx4, cy4, radius_4, radius_4, color);
    aacircleColor(renderer, cx4, cy4, radius_4, color);

    SDL_SetRenderDrawColor(renderer, color_.r(), color_.g(), color_.b(), color_.a());


    filledPolygonColor(renderer, point_x, point_y, 12, color);

//    SDL_RenderFillRect(renderer, &r1);
//    SDL_RenderFillRect(renderer, &r2);
}

void Draw::gradientRect(SDL_Renderer* renderer, const SimpleRect& rect, const Color& start, const Color& end)
{
    const int start_color_r = start.r();
    const int start_color_g = start.g();
    const int start_color_b = start.b();
    const int start_color_a = start.a();


    const int end_color_r = end.r();
    const int end_color_g = end.g();
    const int end_color_b = end.b();
    const int end_color_a = end.a();


    const int count_step = rect.h;


    int step_r = (Utils::max_of(start_color_r, end_color_r) - Utils::min_of(start_color_r, end_color_r)) / count_step;
    int step_g = (Utils::max_of(start_color_g, end_color_g) - Utils::min_of(start_color_g, end_color_g)) / count_step;
    int step_b = (Utils::max_of(start_color_b, end_color_b) - Utils::min_of(start_color_b, end_color_b)) / count_step;
    int step_a = (Utils::max_of(start_color_a, end_color_a) - Utils::min_of(start_color_a, end_color_a)) / count_step;


    int temp_color_r = start_color_r;
    int temp_color_g = start_color_g;
    int temp_color_b = start_color_b;
    int temp_color_a = start_color_a;


    step_r = start_color_r > end_color_r ? -step_r : step_r;
    step_g = start_color_g > end_color_g ? -step_g : step_g;
    step_b = start_color_b > end_color_b ? -step_b : step_b;
    step_a = start_color_a > end_color_a ? -step_a : step_a;

    for (int i = 0; i < count_step; ++i)
    {


        SDL_SetRenderDrawColor(renderer, temp_color_r, temp_color_g, temp_color_b, temp_color_a);
        SDL_RenderDrawLine(renderer, rect.x, rect.y + i, rect.x + rect.w, rect.y + i);




        temp_color_r += step_r;
        temp_color_g += step_g;
        temp_color_b += step_b;
        temp_color_a += step_a;
    }

}
