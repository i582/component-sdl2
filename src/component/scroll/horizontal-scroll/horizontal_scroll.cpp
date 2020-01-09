#include "horizontal_scroll.h"

Kit::horizontal_scroll::horizontal_scroll(Kit::Component* parent_)
        : scroll(parent_) {}

void Kit::horizontal_scroll::shift(int delta_)
{
    scroll::shift(delta_);

    const int new_x_sliderPos = (int)((_body.w - _slider.w) * _nowPosition);
    _slider.x = new_x_sliderPos;
}

void Kit::horizontal_scroll::setup(const Kit::SimpleRect& body_, const Kit::SimpleRect& slider_, int maxValue_,
                                 int nowValue_, double aspectRatio_, double nowPosition_)
{
    scroll::setup(body_, slider_, maxValue_, nowValue_, aspectRatio_, nowPosition_);

    _slider.x = 0;
    _slider.y = 3;

    _slider.w = (int)(_body.w * _aspectRatio);
    _slider.h = _body.h - 6 ;
}
