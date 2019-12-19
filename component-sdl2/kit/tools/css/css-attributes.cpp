#include "css-attributes.h"

using namespace CSS;

string CSS::css_attribute::parseImagePath(string path)
{
	string result;

	result = path.substr(4, path.size() - 5);

	if (result[0] == '"' || result[0] == '\'')
	{
		result = result.substr(1, result.size() - 2);
	}

	return result;
}

css_variant CSS::css_attribute::get(string attribute, string value)
{
	css_variant result;


	if (attribute == "font-size" || attribute == "font-weight" || attribute == "margin-top" || attribute == "margin-bottom"
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

	if (attribute == "background-image")
	{
		return parseImagePath(value);
	}

	result = value;

	return result;
}
