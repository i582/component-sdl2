#include "RadioButton.h"

RadioButton::RadioButton(string id, Rect size, string classes, string name, string text)
	: Component(id, size, classes)
{
	this->callback = [](Component* sender, Event* e) {};
	this->name = name;
	this->text = text;
	this->state = RadioButtonState::UNCHECKED;
	setup();
}

RadioButton::RadioButton(string id, Rect size, string classes, string name, string text, function<void(Component* sender, Event* e)> callback)
	: Component(id, size, classes)
{
	this->callback = callback;
	this->name = name;
	this->text = text;
	this->state = RadioButtonState::UNCHECKED;
	setup();
}

void RadioButton::setup()
{
	this->addEventListener("change", [&](Component* sender, Event* e)
	{
		callback(sender, e);
	});

	this->addEventListener("onmouseup", [&](Component* sender, Event* e)
	{
		if (state == RadioButtonState::CHECKED)
		{
			sender->removeClass(".radio-button-checked");
			state = RadioButtonState::UNCHECKED;
		}
		else
		{
			sender->addClass(".radio-button-checked");
			state = RadioButtonState::CHECKED;
		}

		callEventListener("change", e);
	});

	setText(text);
}

bool RadioButton::isChecked() const
{
	return state == RadioButtonState::CHECKED;
}
