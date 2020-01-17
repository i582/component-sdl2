#pragma once

#include <string>
#include <iostream>

#include "SDL.h"

#include "../utils/utils.h"

namespace CSS
{

    using std::cout;
    using std::endl;
    using std::string;

    class Color
    {
    public:
        Color();

        explicit Color(Uint32 color);

        explicit Color(const string& color);

    public:
        bool operator==(const Color& obj) const;
        bool operator!=(const Color& obj) const;

    public:

        /**
         * @brief Returns color to RGBA
         */
        [[nodiscard]] Uint32 color() const;


        /**
         * @brief Returns color to ABGR
         */
        [[nodiscard]] Uint32 colorReverse() const;


        /**
         * @brief Function for sdl api compatibility
         * @return SDL_Color struct
         */
        [[nodiscard]] SDL_Color colorSDL() const;

        /**
         * @brief
         * @return Color part
         */
        [[nodiscard]] Uint8 r() const;
        [[nodiscard]] Uint8 g() const;
        [[nodiscard]] Uint8 b() const;
        [[nodiscard]] Uint8 a() const;

    private:
        Uint32 _color;
        string s_color;

    private:
        void parse(const string& color);

        static Uint32 parseHEX(const string& color);
        static Uint32 parseRGB(const string& color);
        static Uint32 parseRGBA(const string& color);

    };

}