#pragma once

#define _DEBUG_VERSION_NO

#include "color/color.h"
#include "utils/utils.h"

#include "iostream"
#include "vector"
#include "string"
#include "map"

#include "css_variant/css_variant.h"


namespace CSS
{

using std::string;
using std::map;

class css_attribute
{
private:
	static string parseImagePath(string path);

public:
	static css_variant get(string attribute, string value);

};

}