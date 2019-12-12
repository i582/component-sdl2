#include "color.h"

using namespace CSS;

Color::Color()
{
	this->_color = 0;
	this->s_color = "none";
}

Color::Color(Uint32 color)
{
	this->_color = color;
	this->s_color = "none";
}

Color::Color(string color)
{
	this->_color = 0;
	this->s_color = color;

	calc(color);
}

void Color::calc(string color)
{
	const int size = color.size();

	if (color[0] != '#' ||
		(size != 7 && size != 9))
	{
		std::cout << "Error parse '" << color << "' as hex color" << std::endl;
		return;
	}
		
	this->_color = parseColorHexString(color);
}

Uint32 Color::parseColorHexString(string color)
{
	Uint32 result = 0;
	
	for (size_t i = 1; i < color.size(); i++)
	{
		char symbol = tolower(color[i]);
		int value = 0;

		if (symbol >= '0' && symbol <= '9')
		{
			value = symbol - '0';
		}
		else if (symbol >= 'a' && symbol <= 'f')
		{
			value = symbol - 'a' + 10;
		}

		result += value * (Uint32)(pow(16, (7 - (i-1))));
	}

	if (color.size() == 7)
	{
		result |= 0x000000ff;
	}

	return result;
}

bool Color::operator==(const Color& obj)
{
	return _color == obj._color && s_color == obj.s_color;
}

Uint32 Color::color()
{
	return _color;
}

SDL_Color Color::colorSDL() const
{
	Uint8* c = (Uint8*)&_color;
	const SDL_Color sdl_color = { c[3], c[2], c[1], c[0] };
	return sdl_color;
}

Uint8 Color::r()
{
	Uint8* c = (Uint8*)& _color;
	return c[3];
}

Uint8 Color::g()
{
	Uint8* c = (Uint8*)& _color;
	return c[2];
}

Uint8 Color::b()
{
	Uint8* c = (Uint8*)& _color;
	return c[1];
}

Uint8 Color::a()
{
	Uint8* c = (Uint8*)& _color;
	return c[0];
}
