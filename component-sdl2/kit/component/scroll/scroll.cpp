#include "scroll.h"

using namespace Lib;

Scroll::Scroll(SDL_Renderer* renderer, Rect size, int maxValue, double relSizes)
{
	this->_renderer = renderer;
	this->_texture = nullptr;
	this->_parentTexture = nullptr;
	this->_bodySize = size;
	this->_maxValue = maxValue;
	this->_relSizes = relSizes;

	this->_display = true;
	this->_nowValue = 0;
	this->_position = 0.;
}

Scroll::~Scroll()
{
	SDL_DestroyTexture(_texture);
}

void Scroll::init()
{
	this->_texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _bodySize.w(), _bodySize.h());
	SDL_SetTextureBlendMode(_texture, SDL_BLENDMODE_BLEND);


	_sliderSize = Rect({ 3, 0 }, { _bodySize.w() - 6, (int)(_bodySize.h() * _relSizes) });
}

void Scroll::shift(int delta)
{
	_nowValue += delta;

	if (_nowValue > _maxValue)
		_nowValue = _maxValue;
	if (_nowValue < 0)
		_nowValue = 0;

	_position = _nowValue / (double)_maxValue;

	_sliderSize.y((_bodySize.h() - _sliderSize.h()) * _position);
}

bool Scroll::onHover(Point p)
{
	return p.in(_bodySize);
}

bool Scroll::onHoverSlider(Point& p)
{
	Point pos = p;
	pos = pos - _bodySize.start;

	return pos.in(_sliderSize);
}

void Scroll::render()
{
	if (!_display)
		return;

	SDL_SetRenderTarget(_renderer, _texture);

	SDL_SetRenderDrawColor(_renderer, 0x26, 0x32, 0x38, 0x00);
	SDL_RenderFillRect(_renderer, NULL);

	SDL_SetRenderDrawColor(_renderer, 0x59, 0x5b, 0x5d, 0xff);
	SDL_RenderFillRect(_renderer, &_sliderSize.toSdlRect());

	SDL_SetRenderTarget(_renderer, _parentTexture);

	SDL_RenderCopy(_renderer, _texture, NULL, &_bodySize.toSdlRect());
}
