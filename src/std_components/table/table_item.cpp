#include "table_item.h"

TableItem::TableItem(const string& id, const string& text, const string& classes)
        : Component(id, ".table-item " + classes)
{
    this->text = text;

    setup();
}

void TableItem::setup()
{
    style("../styles/std_components/table/style.css");

    setText(this->text);
}
