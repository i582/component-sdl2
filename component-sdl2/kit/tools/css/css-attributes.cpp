#include "css-attributes.h"

using namespace CSS;

std::variant<int, double, string, Color> CSS::css_attribute::get(string attribute, string value)
{
	std::variant<int, double, string, Color> result;


	if (attribute == "font-size" || attribute == "margin-top" || attribute == "margin-bottom"
		|| attribute == "margin-left" || attribute == "margin-right" || attribute == "border-radius"
		|| attribute == "background-position-x" || attribute == "background-position-y")
	{
		return std::stoi(value);
	}

	if (attribute == "line-height")
	{
		return std::stod(value);
	}

	if (attribute == "background-color" || attribute == "border-color" || attribute == "color")
	{
		return Color(value);
	}

	result = value;

	return result;
}
