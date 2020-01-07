#include "select_item.h"

SelectItem::SelectItem(const string& id, const string& text, const string& classes)
        : Component(id, classes)
{
    this->text = text;
    setup();
}

SelectItem* SelectItem::create(const string& id, const string& text, const string& classes)
{
    return new SelectItem(id, text, classes);
}

void SelectItem::setup()
{
    style("../src/std_components/select/css/style.css");

    setText(this->text);
}

