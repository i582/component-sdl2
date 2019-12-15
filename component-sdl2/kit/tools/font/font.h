#pragma once

#include "SDL_ttf.h"
#include "iostream"
#include "string"
#include "map"

namespace Lib
{

using std::map;
using std::string;
using std::to_string;
using std::cout;
using std::endl;


class Fonts;

class Font
{
private:
	static string folderPath;

private:
	string name;
	
	static map <string, TTF_Font*> __openedFonts;

public:
	Font();
	Font(string name);

public:
	friend Fonts;

public:

	/*
	 * @brief Sets the path for the root folder with fonts
	 * @note The path must be relative to the exe file, for example,
	 * if the exe file is in the "debug" folder and the project is
	 * in the "nia-framework" folder, then you 
	 * set the path to ../nia-framework/fonts/
	 */
	static void root(string path);


	static Font& create(string name);


	/*
	 * @brief Return ttf font with @size
	 * @param size[in] Font size
	 * @return Pointer to ttf font
	 */
	TTF_Font* at(unsigned int size) const;
	TTF_Font* operator[](unsigned int size);
	TTF_Font* operator()(unsigned int size);
};

}