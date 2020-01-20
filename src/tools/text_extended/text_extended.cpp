#include "text_extended.h"
#include "component.h"

Kit::text::text(Kit::Component* parent_, const std::string& text_, Kit::SimplePoint start_, const Kit::font& font_,
                size_t font_size_, const Kit::color& font_color_, const double& line_height_)
{
    this->_renderer = nullptr;
    this->_texture = nullptr;

    this->_parent = parent_;

    this->_text = text_;
    this->_is_one_line_text = false;

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
    this->_is_focused = false;

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

    _need_update = true;
}

void Kit::text::splitByLines()
{
    _lines.clear();

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

    const size_t text_block_height = _lines.size() * _lines[0]._size.h;

    _text_size.w = max_width;
    _text_size.h = text_block_height;

    _size.w = _parent->innerSize().w();
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
            _size.w,
            line._size.h
        };

        line.render();

        SDL_SetTextureAlphaMod(_texture, 0xa0);
        SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);

        SDL_SetRenderTarget(_renderer, _texture);
        SDL_RenderCopy(_renderer, line._texture, nullptr, &copy);
        SDL_RenderCopy(_renderer, line._texture, nullptr, &copy);
    }


    if (!_lines.empty() && _is_focused)
    {
        renderCursor();
    }
}

void Kit::text::renderCursor()
{
    SDL_SetRenderTarget(_renderer, _texture);


    const int height_line = _lines[0]._size.h;

    // calculate cursor position
    int y = (int) (_cursor_position.y * height_line) + _text_size.y;
    int x = 0;

    const auto& current_line = _lines[_cursor_position.y];

    const string& textBeforeCursor = current_line._text.substr(0, _cursor_position.x);
    TTF_SizeUTF8(_ttf, textBeforeCursor.c_str(), &x, nullptr);

    x += current_line._size.x;

    const SDL_Rect cursorRect = {x, y, 1, height_line};
    SDL_SetRenderDrawColor(_renderer, 0x00, 0x00, 0x00, 0xff);


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


    dropSelection();

    _need_update = true;
}

void Kit::text::mouseButtonUp(SDL_Event* e_, const Kit::Point& mouse)
{
    _is_mouse_pushed = false;

    _need_update = true;
}

void Kit::text::mouseMotion(SDL_Event* e_, const Point& _mouse)
{
    if (!_is_mouse_pushed)
        return;

    SimplePoint mouse = {_mouse.x(), _mouse.y()};


    if (!_is_select)
    {
        _is_select = true;
        _start_select = _end_select = whereIsCursor(mouse);
    }
    else
    {
        _end_select = _cursor_position = whereIsCursor(mouse);
    }

    handleSelection();
}

void Kit::text::textInput(SDL_Event* e_)
{
    auto& current_line = _lines.at(_cursor_position.y);

    current_line.add_text(e_->text.text, _cursor_position.x);

    _cursor_position.x += 1;


    setupSize();

    _need_update = true;
}

void Kit::text::keyDown(SDL_Event* e_)
{
    switch (e_->key.keysym.sym)
    {

        case SDLK_TAB:
        {
            _lines.at(_cursor_position.y).add_text("   ", _cursor_position.x);

            _cursor_position.x += 3;

            setupSize();
            break;
        }

        case SDLK_v:
        {
            if (SDL_GetModState() & KMOD_CTRL)
            {
                const string clipboardText = SDL_GetClipboardText();
                _lines.at(_cursor_position.y).add_text(clipboardText, _cursor_position.x);

                _cursor_position.x += clipboardText.length();

                setupSize();
            }

            break;
        }

        case SDLK_c:
        {
            if (SDL_GetModState() & KMOD_CTRL)
            {
                const string select_text = copySelection();
                SDL_SetClipboardText(select_text.c_str());
            }

            break;
        }

        case SDLK_a:
        {
            if (SDL_GetModState() & KMOD_CTRL)
            {
                if (_start_select == cursor_position(0, 0) &&
                    _end_select ==
                    cursor_position((int) (_lines[_lines.size() - 1]._text.size()), (int) _lines.size() - 1))
                {
                    _start_select = _end_select = {0, 0};

                    _is_select = false;
                }
                else
                {

                    _start_select = {0, 0};

                    _end_select =
                    _cursor_position = {(int) (_lines[_lines.size() - 1]._text.size()), (int) _lines.size() - 1};

                    _is_select = true;
                }

                handleSelection();
            }

            break;
        }

        case SDLK_RETURN:
        {
            if (_is_one_line_text)
            {
                return;
            }

            string new_line_text;


            if (_cursor_position.x != _lines[_cursor_position.y]._text.length())
            {

                new_line_text += _lines[_cursor_position.y]._text.substr(_cursor_position.x,
                                                                 _lines[_cursor_position.y]._text.length() - _cursor_position.x);


                _lines[_cursor_position.y].set_text(_lines[_cursor_position.y]._text.substr(0, _cursor_position.x));
            }
            else
            {
                new_line_text = "";
            }

            auto new_line = text_line(this, new_line_text, _cursor_position.y + 1);

            _lines.insert(_lines.begin() + _cursor_position.y + 1, new_line);

            _cursor_position.x = 0;
            _cursor_position.y += 1;

            setupSize();
            break;
        }

        case SDLK_BACKSPACE:
        {
            if (_is_select)
            {
                deleteSelection();
            }
            else
            {
                bool line_delete = false;


                if (_lines.at(_cursor_position.y).removeSymbol(_cursor_position.x) && _lines.size() > 1)
                {
                    _lines.erase(_lines.begin() + _cursor_position.y);
                    line_delete = true;
                }

                if (_cursor_position.x > 0)
                {
                    _cursor_position.x -= 1;
                }
                else
                {
                    if (_cursor_position.y > 0)
                    {
                        _cursor_position.y -= 1;


                        _cursor_position.x = _lines[_cursor_position.y]._text.size();


                        if (!line_delete)
                        {
                            auto& current_line = _lines[_cursor_position.y];
                            auto& next_line = _lines[_cursor_position.y + 1];

                            current_line.set_text(current_line._text + next_line._text);


                            _lines.erase(_lines.begin() + _cursor_position.y + 1);
                        }

                    }
                }
            }

            setupSize();
            break;
        }

        case SDLK_DELETE:
        {
            if (_is_select)
            {
                deleteSelection();
            }
            else if (_lines.at(_cursor_position.y).removeSymbolAfter(_cursor_position.x) &&
                    _cursor_position.y < _lines.size() - 1)
            {
                _lines.at(_cursor_position.y).set_text(_lines.at(_cursor_position.y)._text + _lines.at(_cursor_position.y + 1)._text);
                _lines.erase(_lines.begin() + _cursor_position.y + 1);
            }

            setupSize();

            break;
        }


        case SDLK_LEFT:
        {
            if (_cursor_position.x > 0)
            {
                _cursor_position.x -= 1;


                if (SDL_GetModState() & KMOD_SHIFT)
                {
                    if (!_is_select)
                    {

                        _start_select = _cursor_position;
                        _start_select.x += 1;


                        _end_select = _cursor_position;

                        _is_select = true;
                    }
                    else
                    {

                        _end_select = _cursor_position;
                    }

                    handleSelection();
                }
                else
                {
                    _is_select = false;


                    dropSelection();
                }
            }
            else
            {
                if (_cursor_position.y > 0)
                {
                    _cursor_position.y -= 1;
                    _cursor_position.x = _lines[_cursor_position.y]._text.size();

                    if (SDL_GetModState() & KMOD_SHIFT)
                    {

                        _end_select = _cursor_position;
                    }
                }

            }

            break;
        }

        case SDLK_RIGHT:
        {
            if (_cursor_position.x < _lines[_cursor_position.y]._text.size())
            {
                _cursor_position.x += 1;

                if (SDL_GetModState() & KMOD_SHIFT)
                {
                    if (_is_select == false)
                    {

                        _start_select = _cursor_position;
                        _start_select.x -= 1;


                        _end_select = _cursor_position;

                        _is_select = true;
                    }
                    else
                    {
                        _end_select = _cursor_position;
                    }

                    handleSelection();
                }
                else
                {
                    _is_select = false;

                    dropSelection();
                }
            }
            else
            {
                if (_cursor_position.y < _lines.size() - 1)
                {
                    _cursor_position.y += 1;
                    _cursor_position.x = 0;

                    if (SDL_GetModState() & KMOD_SHIFT)
                    {

                        _end_select = _cursor_position;
                    }
                    else
                    {
                        _is_select = false;

                        dropSelection();
                    }
                }
            }
            break;
        }

        case SDLK_DOWN:
        {

            if (_cursor_position.y < _lines.size() - 1)
            {

                _cursor_position.y += 1;


                if (_cursor_position.x > _lines[_cursor_position.y]._text.size())
                {

                    _cursor_position.x = _lines[_cursor_position.y]._text.size();
                }
            }

            else if (_cursor_position.y == _lines.size() - 1)
            {

                _cursor_position.x = _lines[_cursor_position.y]._text.size();
            }


            if (SDL_GetModState() & KMOD_SHIFT)
            {
                if (!_is_select)
                {

                    _start_select = _cursor_position;

                    if (_start_select.y > 0)
                    {
                        _start_select.y -= 1;
                    }



                    _end_select = _cursor_position;

                    _is_select = true;
                }
                else
                {

                    _end_select = _cursor_position;
                }

                handleSelection();
            }
            else
            {
                _is_select = false;

                dropSelection();
            }

            break;
        }

        case SDLK_UP:
        {

            if (_cursor_position.y > 0)
            {

                _cursor_position.y -= 1;


                if (_cursor_position.x > _lines[_cursor_position.y]._text.size())
                {
                    _cursor_position.x = _lines[_cursor_position.y]._text.size();
                }

            }
            else if (_cursor_position.y == 0)
            {

                _cursor_position.x = 0;
            }


            if (SDL_GetModState() & KMOD_SHIFT)
            {
                if (!_is_select)
                {
                    _start_select = _cursor_position;

                    if (_start_select.y < this->_lines.size() - 1)
                    {
                        _start_select.y += 1;
                    }



                    _end_select = _cursor_position;

                    _is_select = true;
                }
                else
                {

                    _end_select = _cursor_position;
                }

                handleSelection();
            }
            else
            {
                _is_select = false;
                dropSelection();
            }

            break;
        }

        default:
            break;
    }

    _need_update = true;
    render();
}

void Kit::text::dropSelection()
{
    _is_select = false;

    for (auto& line : _lines)
    {
        line.delete_selection();
    }
}

void Kit::text::handleSelection()
{
    for (auto& line : _lines)
    {
        line.delete_selection();
    }


    cursor_position tempStartCursorSelect = _start_select;
    cursor_position tempEndCursorSelect = _end_select;

    if (tempStartCursorSelect.y > tempEndCursorSelect.y)
    {
        std::swap(tempStartCursorSelect, tempEndCursorSelect);
    }


    if (tempStartCursorSelect.y == tempEndCursorSelect.y)
    {
        _lines[tempStartCursorSelect.y]._is_select = true;

        _lines[tempStartCursorSelect.y]._start_selection.x = tempStartCursorSelect.x;
        _lines[tempStartCursorSelect.y]._end_selection.x = tempEndCursorSelect.x;
    }
    else if (tempEndCursorSelect.y - tempStartCursorSelect.y >= 1)
    {
        _lines[tempStartCursorSelect.y]._is_select = true;
        _lines[tempStartCursorSelect.y]._start_selection.x = tempStartCursorSelect.x;
        _lines[tempStartCursorSelect.y]._end_selection.x = _lines[tempStartCursorSelect.y]._text.size();


        for (size_t i = tempStartCursorSelect.y + 1; i < tempEndCursorSelect.y; i++)
        {
            _lines[i]._is_select = true;
            _lines[i]._start_selection.x = 0;
            _lines[i]._end_selection.x = _lines[i]._text.size();
        }

        _lines[tempEndCursorSelect.y]._is_select = true;
        _lines[tempEndCursorSelect.y]._start_selection.x = 0;
        _lines[tempEndCursorSelect.y]._end_selection.x = tempEndCursorSelect.x;
    }

    _need_update = true;
}

Kit::string Kit::text::copySelection()
{
    if (!_is_select)
        return "";


    string result;

    cursor_position tempStartCursorSelect = _start_select;
    cursor_position tempEndCursorSelect = _end_select;

    if (tempStartCursorSelect.y > tempEndCursorSelect.y)
    {
        std::swap(tempStartCursorSelect, tempEndCursorSelect);
    }


    if (tempStartCursorSelect.y == tempEndCursorSelect.y)
    {
        int start = tempStartCursorSelect.x;
        int end = tempEndCursorSelect.x;

        if (start > end)
        {
            std::swap(start, end);
        }

        result = _lines[tempStartCursorSelect.y]._text.substr(start, end - start);
    }
    else if (tempEndCursorSelect.y - tempStartCursorSelect.y >= 1)
    {
        int start = tempStartCursorSelect.x;
        int end = _lines[tempStartCursorSelect.y]._text.size();

        result += _lines[tempStartCursorSelect.y]._text.substr(start, end - start) + '\n';


        for (size_t i = tempStartCursorSelect.y + 1; i < tempEndCursorSelect.y; i++)
        {
            start = 0;
            end = _lines[i]._text.size();

            result += _lines[i]._text.substr(start, end) + '\n';
        }


        start = 0;
        end = tempEndCursorSelect.x;
        result += _lines[tempEndCursorSelect.y]._text.substr(start, end - start);
    }

    return result;
}

void Kit::text::deleteSelection()
{
    if (!_is_select)
        return;

    cursor_position tempStartCursorSelect = _start_select;
    cursor_position tempEndCursorSelect = _end_select;

    if (tempStartCursorSelect.y > tempEndCursorSelect.y)
    {
        std::swap(tempStartCursorSelect, tempEndCursorSelect);
    }


    if (tempStartCursorSelect.y == tempEndCursorSelect.y)
    {
        bool from_start_to_end = true;

        int start = tempStartCursorSelect.x;
        int end = tempEndCursorSelect.x;

        if (start > end)
        {
            std::swap(start, end);
            from_start_to_end = false;
        }

        auto& current_line = _lines[tempStartCursorSelect.y];


        if (start == 0 && end == current_line._text.size())
        {
            current_line._text.clear();
        }
        else
        {
            const string& result = current_line._text.erase(start, end - start);
            current_line._text = result;

            if (from_start_to_end)
            {
                _cursor_position.x -= (end - start);
            }

        }
    }
    else if (tempEndCursorSelect.y - tempStartCursorSelect.y >= 1)
    {
        auto& first_line = _lines[tempStartCursorSelect.y];

        int start = tempStartCursorSelect.x;
        int end = first_line._text.size();


        string result = first_line._text.erase(start, end - start);
        first_line._text = result;



        for (size_t i = tempStartCursorSelect.y + 1; i < tempEndCursorSelect.y; ++i)
        {
            _lines.erase(_lines.begin() + i);
        }



        auto& last_line = _lines[tempEndCursorSelect.y];
        // start = 0;
        end = tempEndCursorSelect.x;

        result = last_line._text.substr(end);
        last_line._text = result;
    }

    dropSelection();
    _need_update = true;
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

    const auto& current_line = _lines[number_line];

    int calculated_width = 0 + current_line._size.x;
    int count_symbol = 0;

    for (const auto& symbol : current_line._text)
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

    return {count_symbol, number_line};
}

void Kit::text::withText(const std::string& newText)
{
    _text = newText;

    setup();
}

void Kit::text::useFont(const Kit::font& newFont)
{
    if (_font == newFont)
        return;

    _font = newFont;
    _ttf = _font.at(_font_size);


    _need_update = true;
}

void Kit::text::fontSize(const size_t& newFontSize)
{
    if (_font_size == newFontSize)
        return;

    _font_size = newFontSize;
    _ttf = _font.at(_font_size);


    _need_update = true;
}

void Kit::text::useColor(const Kit::color& newColor)
{
    if (_color == newColor)
        return;

    _color = newColor;

    _need_update = true;
}

void Kit::text::lineHeight(const double& lineHeight)
{
    if (_line_height == lineHeight)
        return;


    _line_height = lineHeight;

    _need_update = true;
}

void Kit::text::textHorizontalAlign(const std::string& align)
{
    text_horizontal_align temp = text_horizontal_align::LEFT;

    if (align == "left")
    {
        temp = text_horizontal_align::LEFT;
    }
    else if (align == "center")
    {
        temp = text_horizontal_align::CENTER;
    }
    else if (align == "right")
    {
        temp = text_horizontal_align::RIGHT;
    }

    if (_horizontal_align == temp)
        return;

    _horizontal_align = temp;

    _need_update = true;
}

void Kit::text::textVerticalAlign(const std::string& align)
{
    text_vertical_align temp = text_vertical_align::TOP;
    if (align == "top")
    {
        temp = text_vertical_align::TOP;
    }
    else if (align == "middle")
    {
        temp = text_vertical_align::MIDDLE;
    }
    else if (align == "bottom")
    {
        temp = text_vertical_align::BOTTOM;
    }
    else
    {
        return;
    }

    if (_vertical_align == temp)
        return;

    _vertical_align = temp;

    _need_update = true;
}

void Kit::text::focus(bool focus)
{
    _is_focused = focus;

    _need_update = true;
}
