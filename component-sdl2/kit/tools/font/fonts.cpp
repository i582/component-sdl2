#include "fonts.h"

using namespace Lib;

Fonts* Fonts::instance = nullptr;
map <string, Font> Fonts::__fonts = {};

Fonts* Fonts::getInstance()
{
	if (instance == nullptr)
		instance = new Fonts;

	return instance;
}

void Fonts::add(const Font& font)
{
	string name = font.name;
	__fonts[name] = font;
}

Font& Fonts::at(string name)
{
	if (__fonts.find(name) != __fonts.end())
	{
		return __fonts[name];
	}
	
	cout << "Font with name " << name << " not found!" << endl;

	return *(new Font());
}

void Fonts::close()
{
	for (auto& font : Font::__openedFonts)
	{
		TTF_CloseFont(font.second);
	}
}
