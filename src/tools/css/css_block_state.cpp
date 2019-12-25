#include "css_block_state.h"

CSS::css_block_state::css_block_state()
{
	styles["width"] = 0;
	styles["height"] = 0;
	styles["left"] = 0;
	styles["top"] = 0;

	styles["background-color"] = 0;
	styles["border-color"] = 0;
	styles["color"] = 0;

	styles["background-image"] = 0;
	styles["background-position-x"] = 0;
	styles["background-position-y"] = 0;

	styles["background-size"] = 0;

	styles["font-family"] = 0;
	styles["font-weight"] = 0;
	styles["font-style"] = 0;
	styles["font-size"] = 0;

	styles["line-height"] = 0;
	styles["text-align"] = 0;

	styles["vertical-align"] = 0;
	styles["horizontal-align"] = 0;

	styles["margin-top"] = 0;
	styles["margin-bottom"] = 0;

	styles["margin-left"] = 0;
	styles["margin-right"] = 0;

	styles["border-radius"] = 0;


	styles["border-top"] = 0;
	styles["border-top-size"] = 0;
	styles["border-top-color"] = 0;
	styles["border-top-type"] = 0;

	styles["border-bottom"] = 0;
	styles["border-bottom-size"] = 0;
	styles["border-bottom-color"] = 0;
	styles["border-bottom-type"] = 0;

	styles["border-left"] = 0;
	styles["border-left-size"] = 0;
	styles["border-left-color"] = 0;
	styles["border-left-type"] = 0;

	styles["border-right"] = 0;
	styles["border-right-size"] = 0;
	styles["border-right-color"] = 0;
	styles["border-right-type"] = 0;


	styles["overflow"] = 0;
}

CSS::css_block_state::css_block_state(bool general)
{
	styles["width"] = "";
	styles["height"] = "";
	styles["left"] = "";
	styles["top"] = "";

	styles["background-color"] = Color(0xffffff00);
	styles["border-color"] = Color(0xffffff00);
	styles["color"] = Color(0x000000ff);

	styles["background-image"] = "";
	styles["background-position-x"] = 0;
	styles["background-position-y"] = 0;

	styles["background-size"] = "";

	styles["font-family"] = "Segoe UI";
	styles["font-weight"] = 400;
	styles["font-style"] = "normal";
	styles["font-size"] = 13;

	styles["line-height"] = 1.1;
	styles["text-align"] = "left";

	styles["vertical-align"] = "top";
	styles["horizontal-align"] = "left";

	styles["margin-top"] = 0;
	styles["margin-bottom"] = 0;

	styles["margin-left"] = 0;
	styles["margin-right"] = 0;

	styles["border-radius"] = 0;


	styles["border-top"] = 0;
	styles["border-top-size"] = 0;
	styles["border-top-color"] = 0;
	styles["border-top-type"] = "solid";

	styles["border-bottom"] = 0;
	styles["border-bottom-size"] = 0;
	styles["border-bottom-color"] = 0;
	styles["border-bottom-type"] = "solid";

	styles["border-left"] = 0;
	styles["border-left-size"] = 0;
	styles["border-left-color"] = 0;
	styles["border-left-type"] = "solid";

	styles["border-right"] = 0;
	styles["border-right-size"] = 0;
	styles["border-right-color"] = 0;
	styles["border-right-type"] = "solid";

	styles["overflow"] = "unset";
}

void CSS::css_block_state::mergeWith(css_block_state& block)
{
	map <string, css_variant> mergedStyles;

	for (auto& [attr, thisStyle] : styles)
	{
		auto otherStyle = block.styles.at(attr);
	
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

	this->styles = mergedStyles;
}

void CSS::css_block_state::mergeWithBaseIs(css_block_state& block)
{
	map <string, css_variant> mergedStyles;

	for (auto& [attr, thisStyle] : styles)
	{
		auto otherStyle = block.styles.at(attr);
		
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

	this->styles = mergedStyles;
}