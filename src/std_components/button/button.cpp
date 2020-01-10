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
    style("../styles/std_components/button/style.css");

    setText(text);
}
