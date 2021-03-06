#pragma once

#include "../rect/simple-rect/simple-rect.h"
#include "../rect/extended-rect/extended-rect.h"
#include "../css/color/color.h"
#include "../font/font.h"
#include "iostream"
#include "string"
#include "map"

namespace Kit
{

    using std::map;
    using std::string;
    using std::to_string;
    using std::cout;
    using std::endl;

    class Text;

    class TextLine
    {
    private:
        string text;

        SimpleRect size;

        Text* _parent;

        SDL_Renderer* renderer;
        SDL_Texture* parentTexture;

        SDL_Texture* texture;

        font _font;
        size_t fontSize;
        TTF_Font* fontTTF;
        CSS::Color color;

    public:
        TextLine(Text* parent, string text, int x, int y);


    public:


    };

}