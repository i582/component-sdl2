#include "Checkbox.h"

Checkbox::Checkbox(string id, Rect size, string classes, string text)
	: Component(id, size, classes)
{
	this->callback = [](Component* sender, Event* e) {};
	this->text = text;
	this->state = CheckboxState::UNCHECKED;
	setup();
}

Checkbox::Checkbox(string id, Rect size, string classes, string text, function<void(Component* sender, Event* e)> callback)
	: Component(id, size, classes)
{
	this->callback = callback;
	this->text = text;
	this->state = CheckboxState::UNCHECKED;
	setup();
}


void Checkbox::setup()
{
	this->addEventListener("change", [&](Component* sender, Event* e)
	{
		callback(sender, e);
	});

	this->addEventListener("onmouseup", [&](Component* sender, Event* e)
	{
		if (state == CheckboxState::CHECKED)
		{
			sender->removeClass(".checkbox-checked");
			state = CheckboxState::UNCHECKED;
		}
		else
		{
			sender->addClass(".checkbox-checked");
			state = CheckboxState::CHECKED;
		}

		callEventListener("change", e);
	});

	setText(text);
}

bool Checkbox::isChecked() const
{
	return state == CheckboxState::CHECKED;
}
