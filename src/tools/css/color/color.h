#pragma once

#include "SDL.h"

#include "string"
#include "iostream"

#include "../utils/utils.h"

namespace CSS
{

using std::cout;
using std::endl;
using std::string;

class Color
{
private:
	Uint32 _color;
	string s_color;
	
public:
	Color();
	Color(Uint32 color);
	Color(const string& color);

private:
	void parse(const string& color);

	Uint32 parseHEX(const string& color);
	Uint32 parseRGB(const string& color);
	Uint32 parseRGBA(const string& color);

public:
	bool operator==(const Color& obj) const;
    bool operator!=(const Color& obj) const;

public:

    /**
	 * @brief Returns color to RGBA
	 */
	Uint32 color() const;


	/**
	 * @brief Returns color to ABGR
	 */
    Uint32 colorReverse() const;


	/**
	 * @brief Function for sdl api compatibility
	 * @return SDL_Color struct
	 */
	SDL_Color colorSDL() const;

	/**
	 * @brief 
	 * @return Color part
	 */
	Uint8 r() const;
	Uint8 g() const;
	Uint8 b() const;
	Uint8 a() const;
};

}