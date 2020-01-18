#pragma once

#include "component.h"
#include "table_header.h"
#include "table_row.h"

using namespace Kit;

class Table : public Component
{
private:
    int count_columns;
    int count_row;

public:
    explicit Table(const string& id_, const string& classes_ = "");

public:
    void setup();

public:
    void addColumn(const string& title_);

    void addRow();

    void addRow(const vector<string>& columns_);
};

