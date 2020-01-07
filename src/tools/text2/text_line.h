#pragma once

#include "../texture/texture.h"
#include "../utils/utils.h"

namespace Kit
{

    enum class TokenType
    {
        UNDEFINED,
        TEXT,

        KEYWORD,
        QUOTES,
        NUMBER,
        VALUE,
        OPERATOR
    };

    struct CursorPosition
    {
        int x, y;

        CursorPosition() : x(0), y(0)
        {
        }

        CursorPosition(int x, int y) : x(x), y(y)
        {
        }

        bool operator==(const CursorPosition& obj)
        {
            return this->x == obj.x && this->y == obj.y;
        }
    };

    using std::string;
    using std::vector;

    class Text2;

    class TextLine
    {
    private:
        size_t id;
        string text;

        SimpleSize size;

        Text2* parent;

        SDL_Texture* texture;

        bool isSelect;
        CursorPosition startCursorSelect;
        CursorPosition endCursorSelect;

        int shiftByX;

    public:
        TextLine(Text2* parent, const string& text);

        ~TextLine();


    public:
        friend Text2;

    private:
        void deleteSelected();

    private:
        void setup();

        void render();


        bool removeSymbol(int place);

        bool removeSymbolAfter(int place);

    public:
        void setText(const string& text);

        void addText(const string& additionalText, int place);
    };


}