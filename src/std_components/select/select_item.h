#pragma once

#include "component.h"

using namespace Kit;

class SelectItem : public Component
{
private:
    string text;

public:
    explicit SelectItem(const string& id, const string& text = "Select item", const string& classes = "");

public:
    static SelectItem* create(const string& id, const string& text = "Select", const string& classes = "");

    void setup();

};
