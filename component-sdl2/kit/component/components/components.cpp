#include "components.h"
#include "../component.h"

Lib::Components::Components(vector<Component*>* components)
{
	this->components = components;
}

Lib::Components::~Components()
{
	delete components;
}

void Lib::Components::addEventListener(string action, eventCallback event_callback)
{
	if (event_callback == nullptr || action.empty())
		return;

	for (auto& component : *components)
	{
		component->addEventListener(action, event_callback);
	}
}

void Lib::Components::each(eachCallback each_callback)
{
	if (each_callback == nullptr)
		return;

	for (auto& component : *components)
	{
		each_callback(component);
	}
}

Lib::Component* Lib::Components::at(size_t index)
{
	if (index >= components->size())
		return nullptr;

	return components->at(index);
}

Lib::Component* Lib::Components::operator[](size_t index)
{
	return at(index);
}
