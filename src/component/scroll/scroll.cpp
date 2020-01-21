#include "scroll.h"
#include "component.h"

Kit::scroll::scroll(Component* parent_)
{
    this->_parent = parent_;

    this->_renderer = nullptr;
    this->_texture = nullptr;

    this->_body = { 0, 0, 0, 0 };
    this->_slider = { 0, 0, 0, 0 };

    this->_maxValue = 0;
    this->_nowValue = 0;

    this->_aspectRatio = 0.;


    this->_nowPosition = 0.;

    this->init();
}

Kit::scroll::~scroll()
{
    SDL_DestroyTexture(_texture);
}

void Kit::scroll::init()
{
    _renderer = _parent->renderer();
}

void Kit::scroll::render() const
{
    SDL_SetRenderTarget(_renderer, _texture);

    // draw body
    SDL_SetRenderDrawColor(_renderer, 0x26, 0x32, 0x38, 0x00);
    SDL_RenderClear(_renderer);

    // draw slider
//    SDL_SetRenderDrawColor(_renderer, 0x59, 0x5b, 0x5d, 0xff);
//    SDL_RenderFillRect(_renderer, &_slider);
    Draw::roundedRect(_renderer, _slider, 4, Color(0xD3D2D4ff));


    SDL_SetRenderTarget(_renderer, _parent->outerTexture());

    SDL_RenderCopy(_renderer, _texture, nullptr, &_body);
}

bool Kit::scroll::onHover(const Kit::Point& point_)
{
    const SDL_Point p = point_.toSDLPoint();
    return SDL_PointInRect(&p, &_body);
}

bool Kit::scroll::onSliderHover(const Kit::Point& point_)
{
    SDL_Point p = point_.toSDLPoint();

    p.x -= _body.x;
    p.y -= _body.y;

    return SDL_PointInRect(&p, &_slider);
}

void Kit::scroll::setup(const SimpleRect& body_, const SimpleRect& slider_, int maxValue_, int nowValue_,
        double aspectRatio_, double nowPosition_)
{
    _body = body_;
    _maxValue = maxValue_;
    _nowValue = nowValue_;
    _aspectRatio = aspectRatio_;
    _nowPosition = nowPosition_;

    Texture::destroy(_texture);
    _texture = Texture::create(_renderer, _body);

    _slider = slider_;
}

void Kit::scroll::shift(int delta_)
{
    _nowValue += delta_;

    if (_nowValue > _maxValue)
    {
        _nowValue = _maxValue;
    }

    if (_nowValue < 0)
    {
        _nowValue = 0;
    }

    _nowPosition = _nowValue / (double) _maxValue;
}
