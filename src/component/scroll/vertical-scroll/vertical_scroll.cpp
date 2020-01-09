#include "vertical_scroll.h"

Kit::vertical_scroll::vertical_scroll(Kit::Component* parent_)
    : scroll(parent_) {}

void Kit::vertical_scroll::shift(int delta_)
{
    scroll::shift(delta_);

    const int new_y_sliderPos = (int)((_body.h - _slider.h) * _nowPosition);
    _slider.y = new_y_sliderPos;
}

void Kit::vertical_scroll::setup(const Kit::SimpleRect& body_, const Kit::SimpleRect& slider_, int maxValue_,
                                 int nowValue_, double aspectRatio_, double nowPosition_)
{
    scroll::setup(body_, slider_, maxValue_, nowValue_, aspectRatio_, nowPosition_);

    _slider.h = (int)(_body.h * _aspectRatio);
}
