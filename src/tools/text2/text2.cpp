#include "text.h"
#include "../../component/component.h"

Kit::Text2::Text2(Component* parent, const string& text, SimpleRect size,
                  const class font& new_font, size_t fontSize, const Color& colorFont, const double& lineHeight,
                  bool isOneLine)
{
    if (parent == nullptr)
    {
        cout << "ERROR: Parent is nullptr" << endl;
        return;
    }

    this->renderer = parent->renderer();
    this->texture = nullptr;
    this->size = size;

    this->parent = parent;

    this->text = text;
    this->font = new_font;
    this->fontSize = fontSize;
    this->color = colorFont;
    this->fontTTF = font.at(fontSize);

    this->lineHeight = lineHeight;

    this->isSelected = false;

    this->mousePush = false;

    this->isOneLine = isOneLine;

    this->needUpdate = true;

    this->isFocus = false;

    this->textAlign = TextAlign2::LEFT;
    this->textVerticalAlign = TextBlockVerticalAlign2::TOP;

    this->textMarginTop = 0;
    this->textMarginBottom = 0;
    this->textMarginLeft = 0;
    this->textMarginRight = 0;

    splitByLines();
    setup();
}

Kit::Text2::~Text2()
{
    SDL_DestroyTexture(texture);

    for (auto& line : lines)
    {
        delete line;
    }
}

void Kit::Text2::setup()
{
    const int heightLine = lines[0] == nullptr ? 0 : lines[0]->size.h;

    size.h = (int) (lines.size() * heightLine * lineHeight);

    for (auto& line : lines)
    {
        if (line->size.w > size.w)
        {
            size.w = line->size.w;
        }
    }

    Texture::destroy(texture);
    texture = Texture::create(renderer, size);


    deleteSelect();

    this->needUpdate = true;
}

void Kit::Text2::splitByLines()
{
    if (isOneLine)
    {
        lines.push_back(new TextLine(this, text));
        return;
    }


    vector<string>* text_lines = Utils::split(text, '\n');

    for (auto& line : *text_lines)
    {
        lines.push_back(new TextLine(this, line));
    }

    delete text_lines;
}

void Kit::Text2::handleSelect()
{
    for (auto& line : lines)
    {
        line->deleteSelected();
    }


    CursorPosition tempStartCursorSelect = startCursorSelect;
    CursorPosition tempEndCursorSelect = endCursorSelect;

    if (tempStartCursorSelect.y > tempEndCursorSelect.y)
    {
        std::swap(tempStartCursorSelect, tempEndCursorSelect);
    }


    if (tempStartCursorSelect.y == tempEndCursorSelect.y)
    {
        lines[tempStartCursorSelect.y]->startCursorSelect.x = tempStartCursorSelect.x;
        lines[tempStartCursorSelect.y]->endCursorSelect.x = tempEndCursorSelect.x;
    }

    else if (tempStartCursorSelect.y == tempEndCursorSelect.y - 1)
    {

        lines[tempStartCursorSelect.y]->startCursorSelect.x = tempStartCursorSelect.x;
        lines[tempStartCursorSelect.y]->endCursorSelect.x = lines[tempStartCursorSelect.y]->text.size();


        lines[tempEndCursorSelect.y]->startCursorSelect.x = 0;
        lines[tempEndCursorSelect.y]->endCursorSelect.x = tempEndCursorSelect.x;
    }
    else if (tempEndCursorSelect.y - tempStartCursorSelect.y > 1)
    {

        lines[tempStartCursorSelect.y]->startCursorSelect.x = tempStartCursorSelect.x;
        lines[tempStartCursorSelect.y]->endCursorSelect.x = lines[tempStartCursorSelect.y]->text.size();


        for (size_t i = tempStartCursorSelect.y + 1; i < tempEndCursorSelect.y; i++)
        {
            lines[i]->startCursorSelect.x = 0;
            lines[i]->endCursorSelect.x = lines[i]->text.size();
        }


        lines[tempEndCursorSelect.y]->startCursorSelect.x = 0;
        lines[tempEndCursorSelect.y]->endCursorSelect.x = tempEndCursorSelect.x;
    }

    this->needUpdate = true;
}

void Kit::Text2::deleteSelect()
{
    isSelected = false;

    for (auto& line : lines)
    {
        line->deleteSelected();
    }
}

Kit::string Kit::Text2::copySelect()
{
    if (!isSelected)
        return "";


    string result;

    CursorPosition tempStartCursorSelect = startCursorSelect;
    CursorPosition tempEndCursorSelect = endCursorSelect;

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

        result = lines[tempStartCursorSelect.y]->text.substr(start, end - start);
    }

    else if (tempStartCursorSelect.y == tempEndCursorSelect.y - 1)
    {
        int start = tempStartCursorSelect.x;
        int end = lines[tempStartCursorSelect.y]->text.size();

        result += lines[tempStartCursorSelect.y]->text.substr(start, end - start) + '\n';


        start = 0;
        end = tempEndCursorSelect.x;

        result += lines[tempEndCursorSelect.y]->text.substr(start, end - start);
    }
    else if (tempEndCursorSelect.y - tempStartCursorSelect.y > 1)
    {
        int start = tempStartCursorSelect.x;
        int end = lines[tempStartCursorSelect.y]->text.size();

        result += lines[tempStartCursorSelect.y]->text.substr(start, end - start) + '\n';


        for (size_t i = tempStartCursorSelect.y + 1; i < tempEndCursorSelect.y; i++)
        {
            start = 0;
            end = lines[i]->text.size();

            result += lines[i]->text.substr(start, end) + '\n';
        }


        start = 0;
        end = tempEndCursorSelect.x;
        result += lines[tempEndCursorSelect.y]->text.substr(start, end - start);
    }

    return result;
}

Kit::CursorPosition Kit::Text2::whereIsCursor(SimplePoint mouse)
{
    int numberLine = mouse.y / (int) (fontSize * lineHeight);

    if (numberLine > lines.size() - 1)
    {
        numberLine = (int) lines.size() - 1;
    }

    const auto currentLine = lines[numberLine];

    int calcWidth = lines[numberLine]->shiftByX;
    int countSymbol = 0;

    for (auto& symbol : currentLine->text)
    {
        int widthSymbol = 0;
        char str[2] = {symbol, '\0'};
        TTF_SizeUTF8(fontTTF, str, &widthSymbol, nullptr);


        calcWidth += widthSymbol;
        countSymbol++;


        if (calcWidth > mouse.x)
        {
            const int delta = calcWidth - mouse.x;

            if (delta > widthSymbol / 2.)
            {
                countSymbol--;
            }

            break;
        }
    }

    return {countSymbol, numberLine};
}

void Kit::Text2::renderCursor()
{
    const int heightLine = lines[0] == nullptr ? 0 : lines[0]->size.h;

    // calculate cursor position
    int y = (int) (cursorPos.y * heightLine * lineHeight);
    int x = 0;


    const string& textBeforeCursor = lines[cursorPos.y]->text.substr(0, cursorPos.x);
    TTF_SizeUTF8(fontTTF, textBeforeCursor.c_str(), &x, nullptr);

    x += lines[cursorPos.y]->shiftByX;

    const SDL_Rect cursorRect = {x, y, 1, (int) (heightLine * lineHeight)};
    SDL_SetRenderDrawColor(renderer, 0xdf, 0xdf, 0xbf, 0xff);


    SDL_RenderFillRect(renderer, &cursorRect);
}

void Kit::Text2::update()
{
    SDL_SetRenderTarget(renderer, texture);

    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0x00);
    SDL_RenderClear(renderer);


    if (this->textVerticalAlign == TextBlockVerticalAlign2::TOP)
    {
        size.y = textMarginTop;
    }
    else if (this->textVerticalAlign == TextBlockVerticalAlign2::MIDDLE)
    {
        size.y = (parent->innerSize().h() - size.h) / 2;

        size.y += textMarginTop;
    }
    else if (this->textVerticalAlign == TextBlockVerticalAlign2::BOTTOM)
    {
        size.y = parent->innerSize().h() - size.h;

        size.y -= textMarginBottom;
    }


    for (size_t i = 0; i < lines.size(); i++)
    {
        auto& line = lines[i];

        const int y = (int) (i * line->size.h * lineHeight);

        const SDL_Rect rect = {0, y, size.w, line->size.h};

        line->render();


        SDL_SetRenderTarget(renderer, texture);
        SDL_RenderCopy(renderer, line->texture, nullptr, &rect);
    }


    if (!lines.empty() && isFocus)
    {
        renderCursor();
    }
}

void Kit::Text2::render()
{
    if (needUpdate)
    {
        update();
    }


    SDL_SetRenderTarget(renderer, parent->innerTexture());

    SDL_RenderCopy(renderer, texture, nullptr, &size);
}

void Kit::Text2::setText(const string& new_text)
{
    this->text = new_text;

    splitByLines();
    setup();
}

void Kit::Text2::setSize(const SimpleRect& newSize)
{
    if (SimpleRectFunc::equal(this->size, newSize))
        return;

    this->size = newSize;

    Texture::destroy(texture);
    texture = Texture::create(renderer, newSize);


    this->needUpdate = true;
}

void Kit::Text2::setFont(const class font& newFont)
{
    if (this->font == newFont)
        return;

    this->font = newFont;

    this->fontTTF = font.at(fontSize);


    this->needUpdate = true;
}

void Kit::Text2::setFontSize(const size_t& newFontSize)
{
    if (this->fontSize == newFontSize)
        return;

    this->fontSize = newFontSize;

    this->fontTTF = font.at(fontSize);


    this->needUpdate = true;
}

void Kit::Text2::setColor(const Color& newColor)
{
    if (this->color == newColor)
        return;

    this->color = newColor;

    this->needUpdate = true;
}

void Kit::Text2::setLineHeight(const double& lineHeight)
{
    if (this->lineHeight == lineHeight)
        return;


    this->lineHeight = lineHeight;

    this->needUpdate = true;
}

void Kit::Text2::setTextAlign(const string& align)
{
    TextAlign2 temp = TextAlign2::LEFT;

    if (align == "left")
    {
        temp = TextAlign2::LEFT;
    }
    else if (align == "center")
    {
        temp = TextAlign2::CENTER;
    }
    else if (align == "right")
    {
        temp = TextAlign2::RIGHT;
    }

    if (this->textAlign == temp)
        return;

    this->textAlign = temp;

    this->needUpdate = true;
}

void Kit::Text2::setTextBlockVerticalAlign(const string& align)
{
    TextBlockVerticalAlign2 temp;
    if (align == "top")
    {
        temp = TextBlockVerticalAlign2::TOP;
    }
    else if (align == "middle")
    {
        temp = TextBlockVerticalAlign2::MIDDLE;
    }
    else if (align == "bottom")
    {
        temp = TextBlockVerticalAlign2::BOTTOM;
    }
    else
    {
        return;
    }

    if (this->textVerticalAlign == temp)
        return;

    this->textVerticalAlign = temp;

    this->needUpdate = true;
}

void Kit::Text2::setTextBlockMargin(const string& side, int value)
{
    if (side == "top")
    {
        if (textMarginTop == value)
            return;

        textMarginTop = value;
    }
    else if (side == "bottom")
    {
        if (textMarginBottom == value)
            return;

        textMarginBottom = value;
    }
    else if (side == "left")
    {
        if (textMarginLeft == value)
            return;

        textMarginLeft = value;
    }
    else if (side == "right")
    {
        if (textMarginRight == value)
            return;

        textMarginRight = value;
    }
    else
    {
        return;
    }


    this->needUpdate = true;
}

void Kit::Text2::setFocus(bool focus)
{
    this->isFocus = focus;

    this->needUpdate = true;
}

void Kit::Text2::mouseMotion(SDL_Event* e, const Point& _mouse)
{
    if (!mousePush)
        return;

    SimplePoint mouse = {_mouse.x(), _mouse.y()};


    if (!isSelected)
    {
        isSelected = true;
        startCursorSelect = endCursorSelect = whereIsCursor(mouse);
    }
    else
    {
        endCursorSelect = cursorPos = whereIsCursor(mouse);
    }

    handleSelect();
}

void Kit::Text2::mouseButtonUp(SDL_Event* e, const Point& _mouse)
{
    mousePush = false;

    this->needUpdate = true;
}

void Kit::Text2::mouseButtonDown(SDL_Event* e, const Point& _mouse)
{
    mousePush = true;

    SimplePoint mouse = {_mouse.x(), _mouse.y()};

    cursorPos = whereIsCursor(mouse);

    deleteSelect();

    this->needUpdate = true;
}

void Kit::Text2::mouseButtonDoubleDown(SDL_Event* e)
{
    SimplePoint mouse = {e->motion.x, e->motion.y};
    mouse.x -= size.x;
    mouse.y -= size.y;


    CursorPosition nowCursorPosition = whereIsCursor(mouse);

    auto& now_line = lines[nowCursorPosition.y];


    for (int i = nowCursorPosition.x; i > -1; i--)
    {
        char symbol = now_line->text[i];


    }


    for (size_t i = nowCursorPosition.x; i < now_line->text.size(); i++)
    {
        char symbol = now_line->text[i];


    }

    cursorPos = endCursorSelect;

    handleSelect();
}

void Kit::Text2::keyDown(SDL_Event* e)
{
    switch (e->key.keysym.sym)
    {

        case SDLK_TAB:
        {
            lines.at(cursorPos.y)->addText("   ", cursorPos.x);

            cursorPos.x += 3;

            setup();
            break;
        }

        case SDLK_v:
        {
            if (SDL_GetModState() & KMOD_CTRL)
            {
                const string clipboardText = SDL_GetClipboardText();
                lines.at(cursorPos.y)->addText(clipboardText, cursorPos.x);

                cursorPos.x += clipboardText.length();

                setup();
            }

            break;
        }

        case SDLK_c:
        {
            if (SDL_GetModState() & KMOD_CTRL)
            {
                const string select_text = copySelect();
                SDL_SetClipboardText(text.c_str());
            }

            break;
        }

        case SDLK_a:
        {
            if (SDL_GetModState() & KMOD_CTRL)
            {
                if (startCursorSelect == CursorPosition(0, 0) &&
                    endCursorSelect ==
                    CursorPosition((int) (lines[lines.size() - 1]->text.size()), (int) lines.size() - 1))
                {
                    startCursorSelect = endCursorSelect = {0, 0};

                    isSelected = false;
                }
                else
                {

                    startCursorSelect = {0, 0};

                    endCursorSelect =
                    cursorPos = {(int) (lines[lines.size() - 1]->text.size()), (int) lines.size() - 1};

                    isSelected = true;
                }

                handleSelect();
            }

            break;
        }

        case SDLK_RETURN:
        {
            if (isOneLine)
            {
                return;
            }

            string new_line_text;


            if (cursorPos.x != lines[cursorPos.y]->text.length())
            {

                new_line_text += lines[cursorPos.y]->text.substr(cursorPos.x,
                                                                 lines[cursorPos.y]->text.length() - cursorPos.x);


                lines[cursorPos.y]->setText(lines[cursorPos.y]->text.substr(0, cursorPos.x));
            }

            else
            {
                new_line_text = "";
            }

            auto new_line = new TextLine(this, new_line_text);

            lines.insert(lines.begin() + cursorPos.y + 1, new_line);

            cursorPos.x = 0;
            cursorPos.y += 1;

            setup();
            break;
        }

        case SDLK_BACKSPACE:
        {
            bool line_delete = false;


            if (lines.at(cursorPos.y)->removeSymbol(cursorPos.x) && lines.size() > 1)
            {

                lines.erase(lines.begin() + cursorPos.y);
                line_delete = true;
            }

            if (cursorPos.x > 0)
            {
                cursorPos.x -= 1;
            }
            else
            {
                if (cursorPos.y > 0)
                {
                    cursorPos.y -= 1;


                    cursorPos.x = lines[cursorPos.y]->text.size();


                    if (!line_delete)
                    {

                        lines[cursorPos.y]->setText(lines[cursorPos.y]->text + lines[cursorPos.y + 1]->text);


                        lines.erase(lines.begin() + cursorPos.y + 1);
                    }

                }
            }

            setup();
            break;
        }

        case SDLK_DELETE:
        {

            if (lines.at(cursorPos.y)->removeSymbolAfter(cursorPos.x) && cursorPos.y < lines.size() - 1)
            {

                lines.at(cursorPos.y)->setText(lines.at(cursorPos.y)->text + lines.at(cursorPos.y + 1)->text);


                lines.erase(lines.begin() + cursorPos.y + 1);
            }

            setup();

            break;
        }


        case SDLK_LEFT:
        {
            if (cursorPos.x > 0)
            {
                cursorPos.x -= 1;


                if (SDL_GetModState() & KMOD_SHIFT)
                {
                    if (!isSelected)
                    {

                        startCursorSelect = cursorPos;
                        startCursorSelect.x += 1;


                        endCursorSelect = cursorPos;

                        isSelected = true;
                    }
                    else
                    {

                        endCursorSelect = cursorPos;
                    }

                    handleSelect();
                }
                else
                {
                    isSelected = false;


                    deleteSelect();
                }
            }
            else
            {
                if (cursorPos.y > 0)
                {
                    cursorPos.y -= 1;
                    cursorPos.x = lines[cursorPos.y]->text.size();

                    if (SDL_GetModState() & KMOD_SHIFT)
                    {

                        endCursorSelect = cursorPos;
                    }
                }

            }

            break;
        }

        case SDLK_RIGHT:
        {
            if (cursorPos.x < lines[cursorPos.y]->text.size())
            {
                cursorPos.x += 1;

                if (SDL_GetModState() & KMOD_SHIFT)
                {
                    if (isSelected == false)
                    {

                        startCursorSelect = cursorPos;
                        startCursorSelect.x -= 1;


                        endCursorSelect = cursorPos;

                        isSelected = true;
                    }
                    else
                    {
                        endCursorSelect = cursorPos;
                    }

                    handleSelect();
                }
                else
                {
                    isSelected = false;

                    deleteSelect();
                }
            }
            else
            {
                if (cursorPos.y < lines.size() - 1)
                {
                    cursorPos.y += 1;
                    cursorPos.x = 0;

                    if (SDL_GetModState() & KMOD_SHIFT)
                    {

                        endCursorSelect = cursorPos;
                    }
                    else
                    {
                        isSelected = false;

                        deleteSelect();
                    }
                }
            }
            break;
        }

        case SDLK_DOWN:
        {

            if (cursorPos.y < lines.size() - 1)
            {

                cursorPos.y += 1;


                if (cursorPos.x > lines[cursorPos.y]->text.size())
                {

                    cursorPos.x = lines[cursorPos.y]->text.size();
                }
            }

            else if (cursorPos.y == lines.size() - 1)
            {

                cursorPos.x = lines[cursorPos.y]->text.size();
            }


            if (SDL_GetModState() & KMOD_SHIFT)
            {
                if (!isSelected)
                {

                    startCursorSelect = cursorPos;
                    startCursorSelect.y -= 1;

                    endCursorSelect = cursorPos;

                    isSelected = true;
                }
                else
                {

                    endCursorSelect = cursorPos;
                }

                handleSelect();
            }
            else
            {
                isSelected = false;

                deleteSelect();
            }

            break;
        }

        case SDLK_UP:
        {

            if (cursorPos.y > 0)
            {

                cursorPos.y -= 1;


                if (cursorPos.x > lines[cursorPos.y]->text.size())
                {

                    cursorPos.x = lines[cursorPos.y]->text.size();
                }

            }

            else if (cursorPos.y == 0)
            {

                cursorPos.x = 0;
            }


            if (SDL_GetModState() & KMOD_SHIFT)
            {
                if (!isSelected)
                {

                    startCursorSelect = cursorPos;
                    startCursorSelect.y += 1;

                    endCursorSelect = cursorPos;

                    isSelected = true;
                }
                else
                {

                    endCursorSelect = cursorPos;
                }

                handleSelect();
            }
            else
            {
                isSelected = false;
                deleteSelect();
            }

            break;
        }

        default:
            break;
    }

    this->needUpdate = true;
    render();
}

void Kit::Text2::textInput(SDL_Event* e)
{
    lines.at(cursorPos.y)->addText(e->text.text, cursorPos.x);

    cursorPos.x += 1;

    setup();
    render();
}
