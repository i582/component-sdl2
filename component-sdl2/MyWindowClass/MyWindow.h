#pragma once

#include "../kit/window/window.h"

#include "../MyComponent/MyComponent.h"
#include "../Button/Button.h"
#include "../SystemButton/SystemButton.h"
#include "../Checkbox/Checkbox.h"
#include "../RadioButton/RadioButton.h"
#include "../Label/Label.h"


using namespace Kit;

class MyWindow : public Window
{
private:
	int count_clicks;

public:
	MyWindow(string title, SimpleRect size);

public:
	void setup();

};