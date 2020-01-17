#include "text_3.h"
#include "component.h"

Kit::text::text(Kit::Component* parent_, const std::string& text_, Kit::SimplePoint start_, const Kit::font& font_,
                size_t font_size_, const Kit::color& font_color_, const double& line_height_)
{
    this->_renderer = nullptr;
    this->_texture = nullptr;

    this->_parent = parent_;

    this->_text = text_;

    this->_font = font_;
    this->_font_size = font_size_;
    this->_ttf = nullptr;
    this->_color = font_color_;

    this->_line_height = line_height_;

    this->_horizontal_align = text_horizontal_align::LEFT;
    this->_vertical_align = text_vertical_align::TOP;

    this->_size = { start_.x, start_.y, 0, 0 };
    this->_text_size = {0, 0, 0, 0};

    this->_need_update = true;

    this->_cursor_position = {25, 0};

    this->_is_select = false;
    this->_is_mouse_pushed = false;

    this->setup();
}

Kit::text::~text()
{
    SDL_DestroyTexture(_texture);
}

void Kit::text::setup()
{
    _renderer = _parent->renderer();
    _ttf = _font[_font_size];

    splitByLines();
}

void Kit::text::splitByLines()
{
    vector<string>* text_lines = Utils::split(_text, '\n');

    for (auto& line : *text_lines)
    {
        _lines.emplace_back(this, line, _lines.size());
    }

    delete text_lines;

    setupSize();
}

void Kit::text::setupSize()
{
    if (_lines.empty())
    {
        _size.w = 0;
        _size.h = 0;
        return;
    }

    int max_width = 0;

    for (auto& line : _lines)
    {
        if (line._size.w > max_width)
        {
            max_width = line._size.w;
        }
    }

    const size_t max_height = _lines.size() * _lines[0]._size.h;

    _text_size.w = max_width;
    _text_size.h = max_height;

    _size.w = max_width;
    _size.h = _parent->innerSize().h();


    for (auto& line : _lines)
    {
        line.setup();
    }

    Texture::destroy(_texture);
    _texture = Texture::create(_renderer, _size);
}

void Kit::text::update()
{
    SDL_SetRenderTarget(_renderer, _texture);
    SDL_SetRenderDrawColor(_renderer, 0xff, 0xff, 0xff, 0x00);
    SDL_RenderClear(_renderer);




    int shift_by_y = 0;

    if (_vertical_align == text_vertical_align::TOP)
    {
        shift_by_y = 0;
    }
    else if (_vertical_align == text_vertical_align::MIDDLE)
    {
        shift_by_y = (_parent->innerSize().h() - _text_size.h) / 2;
    }
    else if (_vertical_align == text_vertical_align::BOTTOM)
    {
        shift_by_y = _parent->innerSize().h() - _size.h;
    }


    _text_size.y = shift_by_y;


    for (int i = 0; i < _lines.size(); i++)
    {
        auto& line = _lines[i];

        const int line_shift_by_y = i * line._size.h + shift_by_y;


        const SDL_Rect copy = {
            0,
            line_shift_by_y,
            line._size.w,
            line._size.h
        };

        line.render();


        SDL_SetRenderTarget(_renderer, _texture);
        SDL_RenderCopy(_renderer, line._texture, nullptr, &copy);
    }


    if (!_lines.empty())
    {
        renderCursor();
    }
}

void Kit::text::renderCursor()
{
    SDL_SetRenderTarget(_renderer, _texture);


    const int height_line = _lines[0]._size.h;

    // calculate cursor position
    int y = (int) (_cursor_position.y * height_line);
    int x = 0;

    const auto current_line = _lines[_cursor_position.y];

    const string& textBeforeCursor = current_line._text.substr(0, _cursor_position.x);
    TTF_SizeUTF8(_ttf, textBeforeCursor.c_str(), &x, nullptr);



    cout << "Cursor position: " << x << " " << y << endl;

    const SDL_Rect cursorRect = {x, y, 10, height_line};
    SDL_SetRenderDrawColor(_renderer, 0xdf, 0xdf, 0xbf, 0xff);


    SDL_RenderFillRect(_renderer, &cursorRect);
}

void Kit::text::render()
{
    if (_need_update)
    {
        update();
        _need_update = false;
    }


    SDL_SetRenderTarget(_renderer, _parent->innerTexture());

    SDL_RenderCopy(_renderer, _texture, nullptr, &_size);
}

void Kit::text::mouseButtonDown(SDL_Event* e_, const Kit::Point& mouse_)
{
    _is_mouse_pushed = true;

    SimplePoint mouse = {mouse_.x(), mouse_.y()};


    _cursor_position = whereIsCursor(mouse);


    deleteSelection();

    this->_need_update = true;
}

void Kit::text::mouseButtonUp(SDL_Event* e_, const Kit::Point& mouse)
{
    _is_mouse_pushed = false;

    this->_need_update = true;
}


void Kit::text::deleteSelection()
{
    _is_select = false;

    for (auto& line : _lines)
    {
        line.delete_selection();
    }
}

Kit::cursor_position Kit::text::whereIsCursor(const Kit::SimplePoint& mouse_)
{
    if (_lines.empty())
    {
        return {0, 0};
    }



    int number_line = (mouse_.y - _text_size.y) / _lines[0]._size.h;


    if (number_line > _lines.size() - 1)
    {
        number_line = (int)_lines.size() - 1;
    }

    const auto current_line = _lines[number_line];

    int calculated_width = 0;
    int count_symbol = 0;

    for (auto& symbol : current_line._text)
    {
        int width_symbol = 0;


        const char str[] = { symbol, '\0'};
        TTF_SizeUTF8(_ttf, str, &width_symbol, nullptr);

        calculated_width += width_symbol;
        count_symbol++;



        if (calculated_width > mouse_.x)
        {
            const int delta = calculated_width - mouse_.x;

            if (delta > width_symbol / 2.)
            {
                count_symbol--;
            }


            break;
        }
    }

    cout << "x: " << count_symbol << " y: " << number_line << endl;

    return {count_symbol, number_line};
}