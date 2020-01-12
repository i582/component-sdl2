#pragma once
#include "component.h"
#include "select_window.h"

#include "select_item.h"

namespace Kit
{

    class Select : public Component
    {
    private:
        string _text;


        SimpleRect _windowSize;

        vector<string> _items;

        select_window* _selectWindow;

    public:
        Select(const string& id_, const string& classes_ = "");
        ~Select() override;

    private:
        void handleItems();

    public:
        static Component* create(const string& id_, const string& classes_ = "");

        Select* addItem(const string& item_);

        void setup();
    };

}