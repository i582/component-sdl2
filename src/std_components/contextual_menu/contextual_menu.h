#pragma once

#include "component.h"

using namespace Kit;

class ContextualMenu : public Component
{
private:
    vector<string> _items;

public:
    ContextualMenu(const string& id, const vector<string>& items_, const string& classes = "");

public:
    static Component* create(const string& id, const vector<string>& items_, const string& classes = "");

    void setup();
};