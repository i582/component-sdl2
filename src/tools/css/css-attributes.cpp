#include "css-attributes.h"

using namespace CSS;

string CSS::css_attribute::parseImagePath(const string& path)
{
    string result = path.substr(4, path.size() - 5);

    if (result[0] == '"' || result[0] == '\'')
    {
        result = result.substr(1, result.size() - 2);
    }

    return result;
}

string CSS::css_attribute::parseSizes(const string& path)
{
    string result = path;

    if (result.find("calc") != -1)
    {
        result = result.substr(5, result.size() - 6);
    }

    return result;
}

css_variant CSS::css_attribute::get(const string& attribute, const string& value)
{
    css_variant result;


    if (attribute == "font-size" || attribute == "font-weight" || attribute == "margin-top" ||
        attribute == "margin-bottom"
        || attribute == "margin-left" || attribute == "margin-right" || attribute == "border-radius"
        )
    {
        return std::stoi(value);
    }

    if (attribute == "line-height")
    {
        return std::stod(value);
    }

    if (attribute == "background-color" || attribute == "border-color" || attribute == "color" ||
        attribute == "outline")
    {
        return Color(value);
    }

    if (attribute == "background-image")
    {
        return parseImagePath(value);
    }

    if (attribute == "width" || attribute == "height" || attribute == "top" || attribute == "left")
    {
        return parseSizes(value);
    }

    result = value;

    return value;
}
