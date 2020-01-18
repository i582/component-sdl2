#include "table.h"

Table::Table(const string& id_, const string& classes_)
        : Component(id_, ".table " + classes_)
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

void Table::addColumn(const string& title_)
{
    const auto header = children(_id + "-header");


    const string columns_count = to_string(this->count_columns);

    const string header_item_id = _id + "-header-item-" + columns_count;
    const string header_item_class = ".header-item-" + columns_count + " .column-" + columns_count;
    const string& header_item_text = title_;

    const auto header_item = new TableHeaderItem(header_item_id, header_item_text, header_item_class);


    header->append(header_item);

    count_columns++;
}

void Table::addRow()
{
    const auto body = children(_id + "-body");

    const string row_id = _id + "-row-" + to_string(this->count_row);
    const string row_class = ".row-" + to_string(this->count_row);

    auto row = new TableRow(row_id, row_class);


    body->append(row);

    count_row++;
}

void Table::addRow(const vector<string>& columns_)
{
    const auto body = children(_id + "-body");

    const string row_id = _id + "-row-" + to_string(count_row);
    const string row_class = ".row-" + to_string(count_row) + (count_row % 2 ? " .light" : "");

    const auto row = new TableRow(row_id, row_class);


    body->append(row);


    for (size_t i = 0; i < count_columns && i < columns_.size(); ++i)
    {
        const string items_count = to_string(i);

        const string item_id = _id + "-row-" + to_string(this->count_row) + "-item-" + to_string(i); // NOLINT(performance-inefficient-string-concatenation)
        const string item_class = ".row-item-" + to_string(i) + " .column-" + to_string(i); // NOLINT(performance-inefficient-string-concatenation)
        const string item_text = columns_[i];

        auto item = new TableItem(item_id, item_text, item_class);
        item->ignoreEvents();


        row->append(item);
    }

    count_row++;
}
