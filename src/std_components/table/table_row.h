#pragma once

#include "component.h"
#include "table_item.h"

using namespace Kit;

class Table;

class TableRow : public Component
{
public:
    explicit TableRow(const string& id, const string& classes = "", const vector<Component*>& childrens = {});

public:
    void setup();
};
