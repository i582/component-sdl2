#pragma once

#include "../kit/component/component.h"

using namespace Lib;

class SystemButton : public Component
{
	
public:
	SystemButton(string id, Rect size, string classes);


public:
	void setup();
};