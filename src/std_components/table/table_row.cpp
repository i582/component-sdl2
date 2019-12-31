#include "table_row.h"

TableRow::TableRow(const string& id, const string& classes, const vector<Component*>& childrens)
        : Component(id, ".table-row " + classes, childrens)
{
    setup();
}

void TableRow::setup()
{
    include("../src/std_components/table/css/style.css");


}
