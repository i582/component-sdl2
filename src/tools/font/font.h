#pragma once

#include "SDL_ttf.h"

#include "font-find/font-find.h"

namespace Kit
{
    using std::map;
    using std::string;
    using std::to_string;
    using std::vector;


    class font
    {
    public:
        /**
         * @brief Standard constructor. The default font is set (Segoe UI).
         *
         * @note All fonts are searched in the system folder and in project folders.
         */
        font();

        /**
         * @brief Main constructor
         * @param name Font name
         * @param style Font style [NORMAL | ITALIC]
         * @param weight Font weight [ULTRATHIN, THIN, LIGHT, REGULAR, MEDIUM, SEMIBOLD, BOLD, EXTRABOLD, BLACK]
         * If the font does not have this weight, then the nearest one will be set.
         */
        font(const string& name, font_style style, font_weight weight);

        /**
         * @brief Additional constructor
         * @param name Font name
         * @param style Font style ["normal" | "italic"]
         * @param weight Font weight [100 - 900]
         */
        font(const string& name, const string& style, int weight);


    public:
        /**
         * @brief Function returns a pointer to TTF_Font with the passed size.
         * @param size Font size
         * @return Pointer to TTF_Font with the passed size
         */
        [[nodiscard]] TTF_Font* at(unsigned int size) const;
        [[nodiscard]] TTF_Font* operator[](unsigned int size) const;

        bool operator==(const font& font);
        font& operator=(const font& font);

        /**
         * @brief Function clearing cache.
         */
        static void delete_cache();


    private:
        string path;

    private:
        static map<string, TTF_Font*> _cache;

    private:
        void init();

    };

}