#include "navigator.h"
#include "../../window/window.h"

Kit::Navigator::Navigator(Window* parent)
	: Component("#navigator", { 1, 1, parent->width() - 1, parent->height() - 1 }, ".navigator")
{
	this->_window = parent;
	this->_renderer = parent->getRenderer();
}

void Kit::Navigator::update()
{
	if (_window != nullptr)
	{
		_window->render();
	}
}
