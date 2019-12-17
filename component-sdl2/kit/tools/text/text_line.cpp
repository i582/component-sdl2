#include "text_line.h"
#include "text.h"

using namespace Kit;

TextLine::TextLine(Text* parent, string text, int x, int y)
{
	if (parent == nullptr)
		return;

	this->parent = parent;
	this->text = text;

	this->renderer = parent->getRenderer();
	this->parentTexture = parent->getTexture();

	this->font = parent->getFont();
	this->fontSize = parent->getFontSize();
	this->color = parent->getColor();
	this->fontTTF = font->at(fontSize);

	TTF_SizeUTF8(fontTTF, text.c_str(), &this->size.w, &this->size.h);
	this->size.x = x;
	this->size.y = y;

	this->texture = nullptr;
}
