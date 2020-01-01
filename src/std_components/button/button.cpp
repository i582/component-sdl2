#include "button.h"

Button::Button(const string& id, const string& text, const string& classes)
	: Component(id, ".button " + classes)
{
	this->text = text;
	setup();
}

Component* Button::create(const string& id, const string& text, const string& classes)
{
	return new Button(id, text, classes);
}

void Button::setup()
{
	include("../src/std_components/button/css/button.css");

	setText(text);
}
