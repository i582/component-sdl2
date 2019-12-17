#pragma once

#include "../kit/component/component.h"

using namespace Kit;

class MyComponent : public Component
{
public:
	MyComponent(string id, Rect size, string classes);
	// MyComponent(string id, Rect size, string classes, vector<Component*> childrens);

public:
	void setup();
};