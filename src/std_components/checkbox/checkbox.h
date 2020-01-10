#pragma once

#include "component.h"

using namespace Kit;

enum class CheckboxState
{
    CHECKED,
    UNCHECKED
};

class Checkbox : public Component
{
private:
    string text;

    CheckboxState state;

    function<void(Component* sender, Event* e_)> callback;

public:
    explicit Checkbox(const string& id, const string& text = "Checkbox", const string& classes = ".checkbox",
                      function<void(Component* sender, Event* e_)> callback = {});

public:
    static Checkbox* create(const string& id, const string& text = "Checkbox", const string& classes = ".checkbox",
                            function<void(Component* sender, Event* e_)> callback = [](Component* sender, Event* e_)
                            {
                            });

public:
    void setup();


    bool isChecked() const;
};