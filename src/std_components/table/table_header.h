#pragma once

#include "component.h"
#include "table_header_item.h"

using namespace Kit;

class Table;

class TableHeader : public Component
{
public:
    explicit TableHeader(const string& id, const string& classes = "");

public:
    void setup();
};
