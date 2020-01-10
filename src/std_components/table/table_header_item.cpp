#include "table_header_item.h"

TableHeaderItem::TableHeaderItem(const string& id, const string& text, const string& classes)
        : Component(id, ".table-header-item " + classes)
{
    this->text = text;

    setup();
}

void TableHeaderItem::setup()
{
    style("../styles/std_components/table/style.css");

    setText(this->text);
}
