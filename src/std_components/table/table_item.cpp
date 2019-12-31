#include "table_item.h"

TableItem::TableItem(const string& id, const string& text, const string& classes)
        : Component(id, ".table-item " + classes)
{
    this->text = text;

    setup();
}

void TableItem::setup()
{
    include("../src/std_components/table/css/style.css");

    setText(this->text);
}
