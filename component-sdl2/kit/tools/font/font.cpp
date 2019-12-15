#include "font.h"

using namespace Lib;

string Font::folderPath = "none";
map <string, TTF_Font*> Font::__openedFonts = {};

Font::Font()
{
	this->name = "";
}

Font::Font(string name)
{
	if (!TTF_WasInit())
	{
		if (TTF_Init() == -1)
		{
			cout << "Error: TTF wasn't init!" << endl;
			return;
		}
	}


	this->name = name;
}

void Font::root(string path)
{
	folderPath = path;
}

Font& Font::create(string name)
{
	return *(new Font(name));
}

TTF_Font* Font::at(unsigned int size) const
{
	if (folderPath == "none")
	{
		cout << "Root path not setted! See Font::root for set-up!" << endl;
		return nullptr;
	}

	string path = folderPath + name + ".ttf";
	TTF_Font* font = nullptr;

	if (__openedFonts.find(path + "@" + to_string(size)) != __openedFonts.end())
	{
		font = __openedFonts[path + "@" + to_string(size)];
	}
	else
	{
		font = TTF_OpenFont(path.c_str(), size);
	}

	if (font == nullptr)
	{
		cout << "Error open font with name equal " << name << " from " << path << "!" << endl;
		cout << TTF_GetError() << endl;
		return nullptr;
	}

	__openedFonts[path + "@" + to_string(size)] = font;

	return font;
}

TTF_Font* Font::operator[](unsigned int size)
{
	return at(size);
}

TTF_Font* Font::operator()(unsigned int size)
{
	return at(size);
}
