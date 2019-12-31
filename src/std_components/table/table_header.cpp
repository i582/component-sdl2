#include "table_header.h"

TableHeader::TableHeader(const string& id, const string& classes)
        : Component(id, ".table-header " + classes)
{
    setup();
}

void TableHeader::setup()
{
    include("../src/std_components/table/css/style.css");


}
