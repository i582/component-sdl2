#pragma once

#include "../kit/component/component.h"

using namespace Kit;

class Button : public Component
{
private:
	string text;

public:
	Button(string id, string text = "Button", string classes = "");

public:
	static Component* create(string id, string text = "Button", string classes = "");
		
	void setup();
};