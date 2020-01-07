#include "table_header_item.h"

TableHeaderItem::TableHeaderItem(const string& id, const string& text, const string& classes)
        : Component(id, ".table-header-item " + classes)
{
    this->text = text;

    setup();
}

void TableHeaderItem::setup()
{
    style("../src/std_components/table/css/style.css");

    setText(this->text);
}
