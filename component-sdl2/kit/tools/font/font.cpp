#include "font.h"

using namespace Lib;

map <string, TTF_Font*> Font::__openedFonts = {};


#ifdef __unix__

	string Font::folderPath = "usr/share/fonts/truetype/";
	bool Font::standartFolder = true;
	bool Font::isWindows = false;

#elif defined(_WIN32) || defined(WIN32) 

	string Font::folderPath = ":/Windows/Fonts/";
	bool Font::standartFolder = true;
	bool Font::isWindows = true;

#elif defined(_MAC)

	string Font::folderPath = "~/Library/Fonts/";
	bool Font::standartFolder = true;
	bool Font::isWindows = false;

#endif

Font::Font() : Font("") {}

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
	standartFolder = false;
}

Font& Font::create(string name)
{
	return *(new Font(name));
}

TTF_Font* Font::at(unsigned int size) const
{
	vector<string> extensions = { ".ttf", ".otf" };

	TTF_Font* font = nullptr;

	bool notFound = true;

	if (Font::standartFolder)
	{
		if (isWindows)
		{
			for (char i = 'a'; i <= 'z' && notFound; i++)
			{
				for (auto& extension : extensions)
				{
					string path = i + folderPath + name + extension;

					font = TTF_OpenFont(path.c_str(), size);


					if (font == nullptr && i == 'z' && extension == ".otf")
					{
						cout << "Error open font with name equal " << name << " from " << path << "!" << endl;
						cout << TTF_GetError() << endl;
						return nullptr;
					}
					else if (font != nullptr)
					{
						__openedFonts[path + "@" + to_string(size)] = font;
						notFound = false;
						break;
					}
				}
			}
		}
		else
		{
			for (auto& extension : extensions)
			{
				string path = folderPath + name + extension;

				font = TTF_OpenFont(path.c_str(), size);


				if (font == nullptr && extension == ".otf")
				{
					cout << "Error open font with name equal " << name << " from " << path << "!" << endl;
					cout << TTF_GetError() << endl;
					return nullptr;
				}
				else if (font != nullptr)
				{
					__openedFonts[path + "@" + to_string(size)] = font;
					break;
				}
			}
		}
	}

	for (auto& extension : extensions)
	{
		string path = folderPath + name + extension;

		font = TTF_OpenFont(path.c_str(), size);


		if (font == nullptr && extension == ".otf")
		{
			cout << "Error open font with name equal " << name << " from " << path << "!" << endl;
			cout << TTF_GetError() << endl;
			return nullptr;
		}
		else if (font != nullptr)
		{
			__openedFonts[path + "@" + to_string(size)] = font;
			break;
		}
	}


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
