#pragma once

#include "../kit/window/window.h"

#include "../MyComponent/MyComponent.h"
#include "../Button/Button.h"

using namespace Lib;

class MyWindow : public Window
{
public:
	MyWindow(string title, SimpleRect size);

public:
	void setup();

};