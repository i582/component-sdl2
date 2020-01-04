#pragma once

#include "component.h"
#include "select_item.h"

using namespace Kit;

class Select : public Component
{
private:
    string text;
    string classname;

public:
    explicit Select(const string& id, const string& text = "Select", const string& classes = ".select");

public:
    static Select* create(const string& id, const string& text = "Select", const string& classes = ".select");

    void setup();

    Component* append(Component* component) override;
};
