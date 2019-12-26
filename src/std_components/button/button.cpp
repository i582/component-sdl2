#include "button.h"

Button::Button(string id, string text, string classes)
	: Component(id, ".button " + classes)
{
	this->text = text;
	setup();
}

Component* Button::create(string id, string text, string classes)
{
	return new Button(id, text, ".button " + classes);
}

void Button::setup()
{
	include("../src/std_components/button/css/button.css");

	setText(text);
}
