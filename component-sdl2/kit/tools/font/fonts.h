#pragma once

#include "font.h"

#include "map"

namespace Lib
{

using std::map;

class Fonts
{
private:
	static Fonts* instance;

private:
	static Fonts* getInstance();
	
private:
	static map <string, Font> __fonts;

public:

	/*
	 * @brief Add new font
	 */
	static void add(const Font& font);

	/*
	 * @brief Add new font
	 */
	static Font& at(string name);

	/*
	 * @brief Close all fonts
	 */
	static void close();
};

}