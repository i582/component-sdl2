#include "css_block_state.h"

CSS::css_block_state::css_block_state()
{
    _styles["width"] = 0;
    _styles["height"] = 0;
    _styles["left"] = 0;
    _styles["top"] = 0;

    _styles["background-color"] = 0;
    _styles["border-color"] = 0;
    _styles["color"] = 0;

    _styles["background-image"] = 0;
    _styles["background-position-x"] = 0;
    _styles["background-position-y"] = 0;

    _styles["background-size"] = 0;

    _styles["background"] = 0;
    _styles["background-linear-gradient-start-color"] = 0;
    _styles["background-linear-gradient-end-color"] = 0;

    _styles["font-family"] = 0;
    _styles["font-weight"] = 0;
    _styles["font-style"] = 0;
    _styles["font-size"] = 0;

    _styles["line-height"] = 0;
    _styles["text-align"] = 0;

    _styles["vertical-align"] = 0;
    _styles["horizontal-align"] = 0;

    _styles["margin-top"] = 0;
    _styles["margin-bottom"] = 0;
    _styles["margin-left"] = 0;
    _styles["margin-right"] = 0;

    _styles["border-radius"] = 0;
    _styles["border-radius-top-left"] = 0;
    _styles["border-radius-top-right"] = 0;
    _styles["border-radius-bottom-left"] = 0;
    _styles["border-radius-bottom-right"] = 0;


    _styles["border-top"] = 0;
    _styles["border-top-size"] = 0;
    _styles["border-top-color"] = 0;
    _styles["border-top-type"] = 0;

    _styles["border-bottom"] = 0;
    _styles["border-bottom-size"] = 0;
    _styles["border-bottom-color"] = 0;
    _styles["border-bottom-type"] = 0;

    _styles["border-left"] = 0;
    _styles["border-left-size"] = 0;
    _styles["border-left-color"] = 0;
    _styles["border-left-type"] = 0;

    _styles["border-right"] = 0;
    _styles["border-right-size"] = 0;
    _styles["border-right-color"] = 0;
    _styles["border-right-type"] = 0;

    _styles["outline"] = 0;

    _styles["overflow"] = 0;

    _styles["display"] = 0;


    _styles["padding-top"] = 0;
    _styles["padding-bottom"] = 0;
    _styles["padding-left"] = 0;
    _styles["padding-right"] = 0;
}

CSS::css_block_state::css_block_state(bool general)
{
    _styles["width"] = "0px";
    _styles["height"] = "0px";
    _styles["left"] = "0px";
    _styles["top"] = "0px";

    _styles["background-color"] = Color(0xffffff00);
    _styles["border-color"] = Color(0x00000000);
    _styles["color"] = Color(0x000000ff);

    _styles["background-image"] = "";
    _styles["background-position-x"] = "0px";
    _styles["background-position-y"] = "0px";

    _styles["background-size"] = "0px";

    _styles["background"] = "";
    _styles["background-linear-gradient-start-color"] = Color(0x00000000);
    _styles["background-linear-gradient-end-color"] = Color(0x00000000);



    _styles["font-family"] = "Segoe UI";
    _styles["font-weight"] = 400;
    _styles["font-style"] = "normal";
    _styles["font-size"] = 13;

    _styles["line-height"] = 1.1;
    _styles["text-align"] = "left";

    _styles["vertical-align"] = "top";
    _styles["horizontal-align"] = "left";

    _styles["margin-top"] = 0;
    _styles["margin-bottom"] = 0;
    _styles["margin-left"] = 0;
    _styles["margin-right"] = 0;

    _styles["border-radius"] = 0;
    _styles["border-radius-top-left"] = 0;
    _styles["border-radius-top-right"] = 0;
    _styles["border-radius-bottom-left"] = 0;
    _styles["border-radius-bottom-right"] = 0;

    _styles["border-top"] = 0;
    _styles["border-top-size"] = 0;
    _styles["border-top-color"] = 0;
    _styles["border-top-type"] = "solid";

    _styles["border-bottom"] = 0;
    _styles["border-bottom-size"] = 0;
    _styles["border-bottom-color"] = 0;
    _styles["border-bottom-type"] = "solid";

    _styles["border-left"] = 0;
    _styles["border-left-size"] = 0;
    _styles["border-left-color"] = 0;
    _styles["border-left-type"] = "solid";

    _styles["border-right"] = 0;
    _styles["border-right-size"] = 0;
    _styles["border-right-color"] = 0;
    _styles["border-right-type"] = "solid";

    _styles["outline"] = Color(0x00000000);

    _styles["overflow"] = "unset";


    _styles["display"] = "unset";

    _styles["padding-top"] = 0;
    _styles["padding-bottom"] = 0;
    _styles["padding-left"] = 0;
    _styles["padding-right"] = 0;
}

void CSS::css_block_state::mergeWith(css_block_state& block)
{
    map<string, css_variant> mergedStyles;

    for (auto&[attr, thisStyle] : _styles)
    {
        auto otherStyle = block._styles.at(attr);

        if (otherStyle.type() == css_variant_type::INT)
        {
            if (otherStyle.to_int() == 0)
                mergedStyles[attr] = thisStyle;
            else
                mergedStyles[attr] = otherStyle;
        }
        else
        {
            mergedStyles[attr] = otherStyle;
        }
    }

    this->_styles = mergedStyles;
}

void CSS::css_block_state::mergeWithBaseIs(css_block_state& block)
{
    map<string, css_variant> mergedStyles;

    for (auto&[attr, thisStyle] : _styles)
    {
        auto otherStyle = block._styles.at(attr);

        if (thisStyle.type() == css_variant_type::INT)
        {
            if (thisStyle.to_int() == 0)
                mergedStyles[attr] = otherStyle;
            else
                mergedStyles[attr] = thisStyle;
        }
        else
        {
            mergedStyles[attr] = thisStyle;
        }
    }

    this->_styles = mergedStyles;
}


void CSS::css_block_state::set(const string& attribute, const string& value)
{
    if (attribute.empty())
        return;


    if (attribute == "font-size" || attribute == "font-weight" ||
        attribute == "border-radius" ||
        attribute == "border-radius-top-left" ||
        attribute == "border-radius-top-right" ||
        attribute == "border-radius-bottom-left" ||
        attribute == "border-radius-bottom-right" ||
        attribute == "padding-top" ||  attribute == "padding-bottom" ||
        attribute == "padding-left" || attribute == "padding-right" ||
        attribute == "margin-left" || attribute == "margin-right" ||
        attribute == "margin-top" || attribute == "margin-bottom"
    )
    {
        _styles[attribute] = std::stoi(value);
    }
    else if (attribute == "line-height")
    {
        _styles[attribute] = std::stod(value);
    }
    else if (attribute == "background-color" || attribute == "border-color" || attribute == "color" ||
             attribute == "outline")
    {
        _styles[attribute] = Color(value);
    }
    else
    {
        _styles[attribute] = value;
    }
}
