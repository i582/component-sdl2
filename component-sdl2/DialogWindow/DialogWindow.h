#pragma once

#include "../kit/window/window.h"

#include "../MyComponent/MyComponent.h"
#include "../Button/Button.h"

#include "../DialogAnswer/DialogAnswer.h"


using namespace Lib;



class DialogWindow : public Window
{
public:
	DialogWindow(string title, SimpleRect size);

public:
	void setup();

};