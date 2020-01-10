#include "table.h"

Table::Table(const string& id, const string& classes)
        : Component(id, ".table " + classes)
{
    this->count_row = 0;
    this->count_collumns = 0;

    setup();
}

void Table::setup()
{
    style("../styles/std_components/table/style.css");

    append(new TableHeader(_id + "-header", ".header-table"));

    append(new Component(_id + "-body", ".table-body"));
}

void Table::addCollumn(const string& title)
{
    this->count_collumns++;

    _childrens[0]->append(
            new TableHeaderItem(_id + "-header-item-" + to_string(this->count_collumns), title,
                                ".header-item-" + to_string(this->count_collumns))
    );
}

void Table::addRow()
{
    this->count_row++;

    _childrens[1]->append(
            new TableRow(_id + "-row-" + to_string(this->count_row), ".row-" + to_string(this->count_row))
    );

}

void Table::addRow(const vector<string>& collumns)
{
    this->count_row++;

    string is_light;
    if (this->count_row % 2 == 0)
    {
        is_light = " .light";
    }

    auto row = new TableRow(_id + "-row-" + to_string(this->count_row),
                            ".row-" + to_string(this->count_row) + is_light);

    _childrens[1]->append(row);

    for (int i = 0; i < count_collumns && i < collumns.size(); ++i)
    {
        row->append(
                new TableItem(_id + "-row-" + to_string(this->count_row) + "-item-" + to_string(i), collumns[i],
                              ".row-item-" + to_string(i))
        )->ignoreEvents();
    }
}
