#pragma once

#include "component.h"

using namespace Kit;

class Button : public Component
{
private:
    string text;

public:
    Button(const string& id, const string& text = "Button", const string& classes = "");

public:
    static Component* create(const string& id, const string& text = "Button", const string& classes = "");

    void setup();
};