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
    explicit Table(const string& id, const string& classes = "");

public:
    void setup();

public:
    void addColumn(const string& title);

    void addRow();

    void addRow(const vector<string>& columns);
};

