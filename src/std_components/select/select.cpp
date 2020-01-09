#include "select.h"
#include "kit-main.h"

Select::Select(const string& id, const string& text, const string& classes)
        : Component(id, ".select " + classes)
{
    this->text = text;


    if (!classes.empty())
    {
        const auto classes_vector = Utils::split(classes, ' ');
        this->classname = classes_vector->at(0);
        delete classes_vector;
    }

    setup();
}

Select* Select::create(const string& id, const string& text, const string& classes)
{
    return new Select(id, text, classes);
}

void Select::setup()
{
    style("../src/std_components/select/css/style.css");


    Component::append(new Component(_id + "-header", ".select-header " + classname + "-header"))
            ->setText(text);

    Component::append(new Component(_id + "-body", ".select-body " + classname + "-body"));


    _childrens[0]->addEventListener("click", [this](Component* sender, Event* e)
    {
        _childrens[1]->toggleDisplay();

        KitApplication::getInstance()->addWindow(new window("select-window", {100, 100, 200, 200}));
    });
}

Component* Select::append(Component* component)
{
    string newClasses;

    newClasses += ".select-body-item";
    newClasses += " " + classname + "-body-item";
    newClasses += " " + component->classes();

    component->classes(newClasses);

    _childrens[1]->append(component);
    return component;
}
