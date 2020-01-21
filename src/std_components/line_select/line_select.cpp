#include "line_select.h"

LineSelect::LineSelect(const string& id_, const vector<string>& items_, const string& classes_)
        : Component(id_, ".line-select " + classes_)
{
    this->items = items_;

    this->component_items = nullptr;

    if (!classes_.empty())
    {
        auto result = Utils::split(classes_, ' ');

        first_class = result->at(0);

        delete result;
    }

    this->setup();
}

LineSelect::~LineSelect()
{
    delete component_items;
}

Component* LineSelect::create(const string& id_, const vector<string>& items_, const string& classes_)
{
    return new LineSelect(id_, items_, classes_);
}

void LineSelect::setup()
{
    style("../styles/std_components/line_select/style.css");

    component_items = new vector<Component*>;

    for (int i = 0; i < items.size(); ++i)
    {
        const int items_size = items.size();
        const auto& item = items[i];

        const string identifier = id() + "-item-" + to_string(i);
        string classes = ".line-select-item " + first_class + "-item " + first_class + "-item-" + to_string(i) + (i == 0 ? " .line-select-item-selected" : "");


        string add_classes_system = i == 0 ? " .line-select-item-first " : i == items_size - 1 ? " .line-select-item-last " : " ";
        string add_classes_user = i == 0 ? first_class + "-item-first" : i == items_size - 1 ? first_class + "-item-last" : "";

        auto component_item = Component::create(identifier, classes.append(add_classes_system.append(add_classes_user)));
        component_item->setText(item);
        component_item->addUserData("selected", i == 0);


        component_items->push_back(component_item);

        component_item->addEventListener("click", [component_items = component_items](Component* sender, Event* e)
        {
            for (auto& componentItem : *component_items)
            {
                if (componentItem->userData<bool>("selected") == true)
                {
                    componentItem->setUserData("selected", false);
                    componentItem->removeClass(".line-select-item-selected");

                }
            }

            sender->addClass(".line-select-item-selected");
            sender->setUserData("selected", true);
        });

        append(component_item);
    }

}
