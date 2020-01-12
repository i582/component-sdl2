#pragma once
#include "component.h"
#include "select_window.h"

namespace Kit
{

    class SelectItem : public Component
    {
    private:
        string _text;

    public:
        SelectItem(const string& id_, const string& text_, const string& classes_ = "");

    public:
        static Component* create(const string& id_, const string& text_, const string& classes_ = "");

        void setup();
    };

}