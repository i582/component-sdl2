#pragma once

#include "../kit/component/component.h"

using namespace Kit;

enum class CheckboxState
{
	CHECKED,
	UNCHECKED
};

class Checkbox : public Component
{
private:
	string text;

	CheckboxState state;

	function <void(Component* sender, Event* e)> callback;

public:
	Checkbox(string id, Rect size, string classes, string text);
	Checkbox(string id, Rect size, string classes, string text, function <void(Component* sender, Event* e)> callback);

public:
	void setup();

	bool isChecked() const;
};