#include "button.h"

Button::Button(string id, Rect size, string classes, string text)
	: Component(id, size, classes)
{
	this->text = text;
	setup();
}

void Button::setup()
{
	setText(text);
}
