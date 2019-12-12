#pragma once

#include "SDL.h"

#include "string"
#include "iostream"

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
	Color(string color);

private:
	void calc(string color);
	Uint32 parseColorHexString(string color);

public:
	bool operator==(const Color& obj);

public:
	Uint32 color();

	/*
	 * @brief Function for sdl apis compatibility
	 * @return SDL_Color struct
	 */
	SDL_Color colorSDL() const;

	/*
	 * @brief 
	 * @return Ñolor part
	 */
	Uint8 r();
	Uint8 g();
	Uint8 b();
	Uint8 a();
};

}