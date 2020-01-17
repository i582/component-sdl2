#pragma once

#include <string>

#include <SDL.h>
#include <SDL_ttf.h>

#include "../rect/simple-rect/simple-rect.h"
#include "../texture/texture.h"

#include "text_cursor.h"

namespace Kit
{

    using std::string;

    class text;

    class text_line
    {
    public:
        text_line(text* parent_, const string& text_, size_t number_);
        ~text_line();

    public:
        void setup();
        void render();

        void set_text(const string& text_);
        void add_text(const string& additional_text_, size_t place_);

        void delete_selection();

    private: // Parent Part
        text* _parent;

    private: // SDL Part
        SDL_Texture* _texture;

    private:
        SimpleRect _size;

    private:
        string _text;
        size_t _number;


    private: // Cursor Part
        cursor_position _start_selection;
        cursor_position _end_selection;

        bool _is_select;

    private:
        friend text;

    private:
        void setupSize();

    };

}