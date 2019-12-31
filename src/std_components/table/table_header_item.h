#pragma once

#include "component.h"

using namespace Kit;

class Table;

class TableHeaderItem : public Component
{
private:
    string text;

public:
    explicit TableHeaderItem(const string& id, const string& text, const string& classes = "");

public:
    void setup();
};
