#pragma once

#include "../kit/component/component.h"

using namespace Kit;

class Label : public Component
{
private:
	string text;

public:
	Label(string id, Rect size, string classes, string text);
	

public:
	void setup();

	void setText(string text);
};