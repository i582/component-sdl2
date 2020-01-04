#pragma once

#include "component.h"

using namespace Kit;

class Tabs : public Component
{
private:
    size_t tabs;
    size_t selected;
    vector<Component*> tabs_pointer;

public:
    explicit Tabs(const string& id, const string& classes = "");

public:
    static Component* create(const string& id, const string& classes = "");

    void setup();

    Component* add(const string& title);

    void select(size_t tab);
};