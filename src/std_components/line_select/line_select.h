#pragma once

#include "component.h"

using namespace Kit;

class LineSelect : public Component
{
private:
    vector<string> items;
    vector<Component*>* component_items;
    string first_class;

public:
    LineSelect(const string& id, const vector<string>& items, const string& classes = "");
    ~LineSelect() override;

public:
    static Component* create(const string& id, const vector<string>& items, const string& classes = "");

    void setup();
};