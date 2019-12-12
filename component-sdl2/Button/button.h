#pragma once

#include "../kit/component/component.h"

using namespace Lib;

class Button : public Component
{
public:
	Button(string id, Rect size, string classes, string text);
	

public:
	void setup();
};