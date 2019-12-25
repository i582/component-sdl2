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

Color::Color(const string& color)
{
	this->_color = 0;
	this->s_color = color;

	parse(color);
}

void Color::parse(const string& color)
{
	if (color.find("rgba") != -1)
	{
		this->_color = parseRGBA(color);
	}
	else if (color.find("rgb") != -1)
	{
		this->_color = parseRGB(color);
	}
	else if (color.find('#') != -1)
	{
		this->_color = parseHEX(color);
	}
}

Uint32 Color::parseHEX(const string& color)
{
	Uint32 result = 0;
	
	if (color[0] != '#' ||
		(color.size() != 7 && color.size() != 9))
	{
		std::cout << "Error parse '" << color << "' as hex color" << std::endl;
		return 0;
	}

	for (size_t i = 1; i < color.size(); i++)
	{
		char symbol = (char)tolower(color[i]);
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

Uint32 CSS::Color::parseRGB(const string& color)
{
	Uint32 result = 0;

	string raw_color_number = color.substr(4, color.size() - 5);

	auto colors = Utils::split(raw_color_number, ',');

	if (colors->size() != 3)
	{
		std::cout << "Error parse '" << color << "' as rgb color" << std::endl;
		return 0;
	}

	result += Utils::to_integer(colors->at(0)) << 24;
	result += Utils::to_integer(colors->at(1)) << 16;
	result += Utils::to_integer(colors->at(2)) << 8;
	result += 0x000000ff;


	return result;
}

Uint32 CSS::Color::parseRGBA(const string& color)
{
	Uint32 result = 0;

	string raw_color_number = color.substr(5, color.size() - 5);

	auto colors = Utils::split(raw_color_number, ',');

	if (colors->size() != 4)
	{
		std::cout << "Error parse '" << color << "' as rgba color" << std::endl;
		return 0;
	}

	result += Utils::to_integer(colors->at(0)) << 24;
	result += Utils::to_integer(colors->at(1)) << 16;
	result += Utils::to_integer(colors->at(2)) << 8;
	result += (Uint32)(Utils::to_double(colors->at(3)) * 0xff);

	return result;
}

bool Color::operator==(const Color& obj) const
{
	return _color == obj._color && s_color == obj.s_color;
}

Uint32 Color::color() const
{
	return _color;
}

Uint32 Color::colorReverse() const
{
    auto* c = (Uint8*)&_color;

    Uint32 temp_color = c[0] << 24 | c[1] << 16 | c[2] << 8 | c[3]; // NOLINT(hicpp-signed-bitwise)

    return temp_color;
}

SDL_Color Color::colorSDL() const
{
	Uint8* c = (Uint8*)&_color;
	const SDL_Color sdl_color = { c[3], c[2], c[1], c[0] };
	return sdl_color;
}

Uint8 Color::r() const
{
    return ((Uint8*)& _color)[3];
}

Uint8 Color::g() const
{
    return ((Uint8*)& _color)[2];
}

Uint8 Color::b() const
{
    return ((Uint8*)& _color)[1];
}

Uint8 Color::a() const
{
	return ((Uint8*)& _color)[0];
}
