#include "Label.h"

Label::Label(string id, Rect size, string classes, string text)
	: Component(id, size, classes)
{
	this->text = text;
	setup();
}

void Label::setup()
{
	include("./Label/css/label.css");

	append(new Component(_id + "#label", { "0px", "0px", "100%", "100%" }, ".label"))->setText(this->text);

}

void Label::setText(string text)
{
	_childrens[0]->setText(text);
}
