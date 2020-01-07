#include "navigator.h"
#include "../../window/window.h"

Kit::Navigator::Navigator(Window* parent)
        : Component("#navigator", {1, 1, parent->width() - 2, parent->height() - 2}, ".navigator")
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

void Kit::Navigator::setupSize(const Kit::Rect& new_size)
{
    this->_innerSize = _outerSize = new_size;


    this->computeSize();
    this->computeChildrenSize();
    update();
}
