#include "navigator.h"
#include "../../window/window.h"

Lib::Navigator::Navigator(Window* parent)
	: Component("#navigator", { 0, 0, parent->width(), parent->height() }, ".navigator")
{
	this->_window = parent;
	this->_renderer = parent->getRenderer();
}

void Lib::Navigator::update()
{
	if (_window != nullptr)
	{
		_window->render();
	}
}
