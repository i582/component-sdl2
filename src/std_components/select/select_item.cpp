#include "select_item.h"

SelectItem::SelectItem(const string& id_, const string& text_, const string& classes_)
        : Component(id_, ".select-item " + classes_)
{
    this->_text = text_;
    SelectItem::setup();
}

Component* SelectItem::create(const string& id_, const string& text_, const string& classes_)
{
    return new SelectItem(id_, text_, classes_);
}

void SelectItem::setup()
{
    style("../styles/std_components/select/style.css");

    setText(_text);
}
