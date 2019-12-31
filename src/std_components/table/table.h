#pragma once

#include "component.h"
#include "table_header.h"
#include "table_row.h"

using namespace Kit;

class Table : public Component
{
private:
    int count_collumns;
    int count_row;

public:
    explicit Table(const string& id, const string& classes = "");

public:
    void setup();

public:
    void addCollumn(const string& title);
    void addRow();
    void addRow(const vector<string>& collumns);
};

