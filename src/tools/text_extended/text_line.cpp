#include "text_line.h"
#include "text_extended.h"

Kit::text_line::text_line(Kit::text* parent_, const std::string& text_, size_t number_)
{
    this->_parent = parent_;
    this->_texture = nullptr;

    this->_size = {0};

    this->_text = text_;
    this->_number = number_;

    this->_start_selection = {0, 0};
    this->_end_selection = {0, 0};

    this->_is_select = false;

    this->setupSize();
}

Kit::text_line::~text_line()
{
    SDL_DestroyTexture(_texture);
}

void Kit::text_line::setupSize()
{
    TTF_SizeText(_parent->_ttf, _text.c_str(), &_size.w, &_size.h);
}

void Kit::text_line::setup()
{
    setupSize();

    cout << _size.h << endl;

    if (_parent->_horizontal_align == text_horizontal_align::LEFT)
    {
        _size.x = 0;
    }
    else if (_parent->_horizontal_align == text_horizontal_align::CENTER)
    {
        _size.x = _parent->_size.w / 2;
    }
    else if (_parent->_horizontal_align == text_horizontal_align::RIGHT)
    {
        _size.x = _parent->_size.w;
    }


    const int real_line_height = (int) (_size.h * _parent->_line_height);

    _size.h = real_line_height;

    const SimpleRect& textureSize = {0, 0, _parent->_size.w, _size.h};

    Texture::destroy(_texture);
    _texture = Texture::create(_parent->_renderer, textureSize);
}

void Kit::text_line::render()
{
    SDL_SetRenderTarget(_parent->_renderer, _texture);
    SDL_SetRenderDrawColor(_parent->_renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(_parent->_renderer);


    if (_text.empty())
    {
        return;
    }





    SDL_Surface* text_surface = TTF_RenderUTF8_Blended(_parent->_ttf, _text.c_str(), _parent->_color.colorSDL());
    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(_parent->_renderer, text_surface);


    int shift_by_x = 0;

    if (_parent->_horizontal_align == text_horizontal_align::LEFT)
    {
        shift_by_x = 0;
    }
    else if (_parent->_horizontal_align == text_horizontal_align::CENTER)
    {
        shift_by_x = (_parent->_size.w - text_surface->w) / 2;
    }
    else if (_parent->_horizontal_align == text_horizontal_align::RIGHT)
    {
        shift_by_x = _parent->_size.w - text_surface->w;
    }


    const int shift_by_y = (_size.h - text_surface->h) / 2;

    _size.x = shift_by_x;
    _size.y = shift_by_y;


    // render select
    if (_is_select)
    {
        SDL_SetRenderDrawColor(_parent->_renderer, 0xb1, 0xcf, 0xf0, 0xff);

        SDL_Rect selected_rect = {0, 0, 0, _size.h};

        const int start_select = Utils::min_of(_start_selection.x, _end_selection.x);
        const int end_select = Utils::max_of(_start_selection.x, _end_selection.x);


        const string& text_before_select = _text.substr(0, start_select);
        const string& selected_text = _text.substr(start_select, abs(end_select - start_select));


        TTF_SizeUTF8(_parent->_ttf, text_before_select.c_str(), &selected_rect.x, nullptr);
        TTF_SizeUTF8(_parent->_ttf, selected_text.c_str(), &selected_rect.w, nullptr);

        selected_rect.x += shift_by_x;

        SDL_RenderFillRect(_parent->_renderer, &selected_rect);
    }



    // render text
    const SDL_Rect copyRect = {
        shift_by_x,
        shift_by_y,
        text_surface->w,
        text_surface->h
    };
    SDL_RenderCopy(_parent->_renderer, text_texture, nullptr, &copyRect);
    //SDL_RenderCopy(_parent->_renderer, text_texture, nullptr, &copyRect);

    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text_surface);


    SDL_SetRenderTarget(_parent->_renderer, _parent->_texture);
}

void Kit::text_line::set_text(const string& text_)
{
    this->_text = text_;

    setup();
    render();
}

void Kit::text_line::add_text(const std::string& additional_text_, size_t place_)
{
    if (place_ > _text.length())
    {
        place_ = _text.length();
    }

    if (place_ != _text.length())
    {
        const string& left_part = _text.substr(0, place_);
        const string& right_part = _text.substr(place_, _text.length() - place_);

        _text = left_part + additional_text_ + right_part;
    }
    else
    {
        _text += additional_text_;
    }


    setup();
}

void Kit::text_line::delete_selection()
{
    _start_selection = {0, 0};
    _end_selection = {0, 0};

    _is_select = false;
}


bool Kit::text_line::removeSymbol(size_t place)
{
    if (_text.length() > 0)
    {
        if (place > _text.size())
            return false;


        const string& leftPart = _text.substr(0, place > 0 ? (place - 1) : place);
        const string& rightPart = _text.substr(place, _text.length() - place);

        _text = leftPart + rightPart;

        setupSize();

        return false;
    }
    else
    {
        return true;
    }
}

bool Kit::text_line::removeSymbolAfter(size_t place)
{
    if (place < _text.length())
    {
        const string& leftPart = _text.substr(0, place);
        const string& rightPart = _text.substr(place + 1, _text.length() - place);

        _text = leftPart + rightPart;

        setupSize();

        return false;
    }
    else
    {
        return true;
    }
}
