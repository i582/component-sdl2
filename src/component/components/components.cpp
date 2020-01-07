#include "components.h"
#include "../component.h"

Kit::Components::Components(vector<Component*>* components)
{
    this->components = components;
}

Kit::Components::~Components()
{
    delete components;
}

void Kit::Components::addEventListener(string action, eventCallback event_callback)
{
    if (event_callback == nullptr || action.empty())
        return;

    for (auto& component : *components)
    {
        component->addEventListener(action, event_callback);
    }
}

void Kit::Components::each(eachCallback each_callback)
{
    if (each_callback == nullptr)
        return;

    for (auto& component : *components)
    {
        each_callback(component);
    }
}

Kit::Component* Kit::Components::at(size_t index)
{
    if (index >= components->size())
        return nullptr;

    return components->at(index);
}

Kit::Component* Kit::Components::operator[](size_t index)
{
    return at(index);
}
