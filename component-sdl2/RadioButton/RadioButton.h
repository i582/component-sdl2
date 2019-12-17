#pragma once

#include "../kit/component/component.h"

using namespace Kit;

enum class RadioButtonState
{
	CHECKED,
	UNCHECKED
};

class RadioButton : public Component
{
private:
	string text;
	string name;

	RadioButtonState state;

	function <void(Component* sender, Event* e)> callback;

public:
	RadioButton(string id, Rect size, string classes, string name, string text);
	RadioButton(string id, Rect size, string classes, string name, string text, function <void(Component* sender, Event* e)> callback);

public:
	void setup();

	bool isChecked() const;
};