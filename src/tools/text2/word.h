#pragma once

#include <string>
#include <vector>
#include <SDL.h>

#include "simple-size.h"
#include "font.h"
#include "../css/color/color.h"
#include "../utils/utils.h"

namespace Kit
{

using CSS::Color;
using std::string;
using std::vector;

struct Word
{
	string text;
	Color color;

	Word(const string& text, const Color& color);
};


}