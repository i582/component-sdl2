#pragma once

#include "component.h"

using namespace Kit;

class Table;

class TableItem : public Component
{
private:
    string text;

public:
    explicit TableItem(const string& id, const string& text, const string& classes = "");

public:
    void setup();
};
