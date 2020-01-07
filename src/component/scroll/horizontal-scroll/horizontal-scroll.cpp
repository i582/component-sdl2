#include "horizontal-scroll.h"

Kit::HorizontalScroll::HorizontalScroll(SDL_Renderer* renderer, Rect size, int maxValue, double relSizes)
        : Scroll(renderer, size, maxValue, relSizes)
{
}

void Kit::HorizontalScroll::init()
{
    this->_texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _bodySize.w(),
                                       _bodySize.h());
    SDL_SetTextureBlendMode(_texture, SDL_BLENDMODE_BLEND);


    _sliderSize = Rect({0, 3}, {(int) (_bodySize.w() * _relSizes), _bodySize.h() - 6});
}

void Kit::HorizontalScroll::shift(int delta)
{
    _nowValue += delta;

    if (_nowValue > _maxValue)
        _nowValue = _maxValue;
    if (_nowValue < 0)
        _nowValue = 0;

    _position = _nowValue / (double) _maxValue;

    _sliderSize.x((_bodySize.w() - _sliderSize.w()) * _position);
}
