
#include "text_line.h"
#include "text.h"

Kit::TextLine::TextLine(Text2* parent, const string& text)
{
    this->id = 0;
    this->parent = parent;
    this->texture = nullptr;

    this->text = text;

    this->isSelect = false;

    this->shiftByX = 0;

    setup();
    render();
}

Kit::TextLine::~TextLine()
{
    SDL_DestroyTexture(texture);
}

void Kit::TextLine::deleteSelected()
{
    this->startCursorSelect = {0, 0};
    this->endCursorSelect = {0, 0};
}

void Kit::TextLine::setup()
{
    // calculate size
    TTF_SizeUTF8(parent->fontTTF, text.c_str(), &size.w, &size.h);
    size.h = (int) (size.h * parent->lineHeight);

    const SimpleRect& textureSize = {0, 0, parent->size.w, size.h};

    Texture::destroy(texture);
    texture = Texture::create(parent->renderer, textureSize);
    //SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD);
}

void Kit::TextLine::render()
{
    SDL_SetRenderTarget(parent->renderer, texture);
    SDL_SetRenderDrawColor(parent->renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(parent->renderer);

    if (text.empty())
    {
        return;
    }


    SDL_SetRenderDrawColor(parent->renderer, 0x4b, 0x6e, 0xaf, 0xff);


    SDL_Surface* textSurface = TTF_RenderUTF8_Blended(parent->fontTTF, text.c_str(), parent->color.colorSDL());
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(parent->renderer, textSurface);
    SDL_SetTextureBlendMode(textTexture, SDL_BLENDMODE_BLEND);

    int copyRectShiftX = 0;

    if (parent->textAlign == TextAlign2::LEFT)
    {
        copyRectShiftX = parent->textMarginLeft;
    }
    else if (parent->textAlign == TextAlign2::CENTER)
    {
        copyRectShiftX = (parent->size.w - textSurface->w) / 2 + parent->textMarginLeft;
    }
    else if (parent->textAlign == TextAlign2::RIGHT)
    {
        copyRectShiftX = parent->size.w - textSurface->w - parent->textMarginLeft;
    }

    const int copyRectShiftY = (int) (parent->fontSize * parent->lineHeight - parent->fontSize) / 2;

    this->shiftByX = copyRectShiftX;



    // render select
    SDL_Rect selectedRect = {0, 0, 0, size.h};

    const int startSelect = Utils::min_of(startCursorSelect.x, endCursorSelect.x);
    const int endSelect = Utils::max_of(startCursorSelect.x, endCursorSelect.x);

    const string& textBeforeStartSelect = text.substr(0, startSelect);
    const string& selectedText = text.substr(startSelect, abs(startSelect - endSelect));

    TTF_SizeUTF8(parent->fontTTF, textBeforeStartSelect.c_str(), &selectedRect.x, nullptr);
    TTF_SizeUTF8(parent->fontTTF, selectedText.c_str(), &selectedRect.w, nullptr);

    selectedRect.x += copyRectShiftX;

    SDL_RenderFillRect(parent->renderer, &selectedRect);


    const SDL_Rect copyRect = {
            copyRectShiftX, copyRectShiftY, textSurface->w, textSurface->h
    };
    SDL_RenderCopy(parent->renderer, textTexture, nullptr, &copyRect);


    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);


    SDL_SetRenderTarget(parent->renderer, parent->texture);
}

void Kit::TextLine::setText(const string& new_text)
{
    this->text = new_text;

    setup();
    render();
}

void Kit::TextLine::addText(const string& additionalText, size_t place)
{
    if (place > text.length())
    {
        place = text.length();
    }

    if (place != text.length())
    {
        const string& leftPart = text.substr(0, place);
        const string& rightPart = text.substr(place, text.length() - place);

        text = leftPart + additionalText + rightPart;
    }
    else
    {
        text += additionalText;
    }


    setup();
    render();
}

bool Kit::TextLine::removeSymbol(size_t place)
{
    if (text.length() > 0)
    {
        if (place == 0)
            return false;

        const string& leftPart = text.substr(0, place - 1);
        const string& rightPart = text.substr(place, text.length() - place);

        text = leftPart + rightPart;

        setup();
        render();
        return false;
    }
    else
    {
        return true;
    }
}

bool Kit::TextLine::removeSymbolAfter(size_t place)
{
    if (place < text.length())
    {
        const string& leftPart = text.substr(0, place);
        const string& rightPart = text.substr(place + 1, text.length() - place);

        text = leftPart + rightPart;

        setup();
        render();
        return false;
    }
    else
    {
        return true;
    }
}
