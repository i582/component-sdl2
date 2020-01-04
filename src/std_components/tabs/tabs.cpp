#include "tabs.h"

Tabs::Tabs(const string& id, const string& classes)
        : Component(id, ".tabs " + classes)
{
    this->tabs = 0;
    this->selected = 0;


    setup();
}

Component* Tabs::create(const string& id, const string& classes)
{
    return new Tabs(id, classes);
}

void Tabs::setup()
{
    include("../src/std_components/tabs/css/style.css");


    append(new Component(_id + "-header", ".tabs-header"));

    append(new Component(_id + "-body", ".tabs-body"));


}

Component* Tabs::add(const string& title)
{
    auto new_item = new Component(
            _id + "-header-item-" + to_string(tabs),
            ".tabs-header-item .tabs-header-item-" + to_string(tabs) + " .tabs-header-item-selected"
    );

    new_item->setText(title);
    new_item->addUserData("item-number", tabs);

    tabs_pointer.push_back(new_item);


    new_item->addEventListener("click", [this](Component* sender, Event* e)
    {
        auto select_item = std::any_cast<size_t>(sender->userData("item-number"));

        select(select_item);
    });

    _childrens[0]->append(new_item);

    //_childrens[1].append();

    tabs++;

    return nullptr;
}

void Tabs::select(size_t tab)
{
    if (tab > tabs - 1)
        return;


    tabs_pointer[this->selected]->removeClass(".tabs-header-item-selected");
    this->selected = tab;
    tabs_pointer[this->selected]->addClass(".tabs-header-item-selected");

}
