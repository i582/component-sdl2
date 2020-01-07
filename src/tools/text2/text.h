#pragma once

#include <vector>

#include "text_line.h"
#include "simple-rect.h"
#include "simple-point.h"
#include "utils/utils.h"
#include "extended-point.h"
#include "../font/font.h"
#include "../css/color/color.h"

namespace Kit
{

    enum class TextAlign2
    {
        LEFT,
        CENTER,
        RIGHT
    };

    enum class TextBlockVerticalAlign2
    {
        TOP,
        MIDDLE,
        BOTTOM
    };


    class Component;


    using CSS::Color;
    using std::vector;

    class Text2
    {
    private:
        vector<TextLine*> lines;
        string text;

        SimpleRect size;

        SDL_Renderer* renderer;
        SDL_Texture* texture;

        Component* parent;


        font font;
        size_t fontSize;
        TTF_Font* fontTTF;
        Color color;


        TextAlign2 textAlign;
        TextBlockVerticalAlign2 textVerticalAlign;

        int textMarginTop;
        int textMarginBottom;
        int textMarginLeft;
        int textMarginRight;


        double lineHeight;

        CursorPosition cursorPos;

        bool isSelected;
        CursorPosition startCursorSelect;
        CursorPosition endCursorSelect;

        bool mousePush;
        bool isOneLine;
        bool needUpdate;

        bool isFocus;

    public:
        Text2(Component* parent, const string& text, SimpleRect size,
              const class font& new_font, size_t fontSize, const Color& colorFont, const double& lineHeight,
              bool isOneLine = false);


        ~Text2();

    public:
        friend TextLine;
        friend Component;

    private:
        void setup();

        void splitByLines();


        void handleSelect();

        void deleteSelect();

        string copySelect();

        CursorPosition whereIsCursor(SimplePoint p);

        void renderCursor();

        void update();

    public:
        void render();

        void setText(const string& text);

        void setSize(const SimpleRect& newSize);

        void setFont(const class font& newFont);

        void setFontSize(const size_t& newFontSize);

        void setColor(const Color& newColor);

        void setLineHeight(const double& lineHeight);

        void setTextAlign(const string& align);

        void setTextBlockVerticalAlign(const string& align);

        void setTextBlockMargin(const string& side, int value);

        void setFocus(bool focus);


        void mouseMotion(SDL_Event* e, const Point& mouse);

        void mouseButtonUp(SDL_Event* e, const Point& mouse);

        void mouseButtonDown(SDL_Event* e, const Point& mouse);

        void mouseButtonDoubleDown(SDL_Event* e);

        void keyDown(SDL_Event* e);

        void textInput(SDL_Event* e);

    };


}