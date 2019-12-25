#pragma once

#include "window.h"


#include "button.h"

using namespace Kit;

class SimpleWindow : public Window
{
public:
	SimpleWindow(string title, SimpleRect size);

public:
	void setup();

};