#include "contextual_menu.h"

ContextualMenu::ContextualMenu(const string& id, const vector<string>& items_, const string& classes)
        : Component(id, ".contextual-menu " + classes)
{
    this->_items = items_;
    setup();
}

Component* ContextualMenu::create(const string& id, const vector<string>& items_, const string& classes)
{
    return new ContextualMenu(id, items_, classes);
}

void ContextualMenu::setup()
{
    style("../styles/std_components/contextual_menu/style.css");

    auto body = append(Component::create(id() + "-body", ".contextual-menu-body"));


    for (int i = 0; i < _items.size(); ++i)
    {
        auto& item = _items[i];

        string item_id = id() + "-body-item-" + to_string(i);
        string item_classes = ".contextual-menu-body-item .contextual-menu-body-item-" + to_string(i);

        if (item == "----")
        {
            item_id = id() + "-body-separator-" + to_string(i);
            item_classes = ".contextual-menu-body-separator";
        }

        auto component_item = Component::create(item_id, item_classes);
        component_item->setText(item);

        body->append(component_item);
    }

}
