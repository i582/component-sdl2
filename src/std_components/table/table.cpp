#include "table.h"

Table::Table(const string& id, const string& classes)
        : Component(id, ".table " + classes)
{
    this->count_row = 0;
    this->count_columns = 0;

    setup();
}

void Table::setup()
{
    style("../styles/std_components/table/style.css");

    append(new TableHeader(_id + "-header", ".header-table"));

    append(new Component(_id + "-body", ".table-body"));
}

void Table::addColumn(const string& title)
{
    this->count_columns++;

    const auto header = children(_id + "-header");

    const string header_item_id = _id + "-header-item-" + to_string(this->count_columns);
    const string header_item_class = ".header-item-" + to_string(this->count_columns);
    const string& header_item_text = title;

    const auto header_item = new TableHeaderItem(header_item_id, header_item_text, header_item_class);


    header->append(header_item);
}

void Table::addRow()
{
    this->count_row++;

    const auto body = children(_id + "-body");

    const string row_id = _id + "-row-" + to_string(this->count_row);
    const string row_class = ".row-" + to_string(this->count_row);

    auto row = new TableRow(row_id, row_class);


    body->append(row);
}

void Table::addRow(const vector<string>& columns)
{
    this->count_row++;

    const auto body = children(_id + "-body");

    const string row_id = _id + "-row-" + to_string(this->count_row);
    const string row_class = ".row-" + to_string(this->count_row) + (count_row % 2 ? " .light" : "");

    const auto row = new TableRow(row_id, row_class);


    body->append(row);


    for (int i = 0; i < count_columns && i < columns.size(); ++i)
    {
        const string item_id = _id + "-row-" + to_string(this->count_row) + "-item-" + to_string(i);
        const string item_class = ".row-item-" + to_string(i);
        const string item_text = columns[i];

        auto item = new TableItem(item_id, item_text, item_class);
        item->ignoreEvents();


        row->append(item);
    }
}
