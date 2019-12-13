#pragma once

#include "../kit/window/window.h"

#include "../MyComponent/MyComponent.h"
#include "../Button/Button.h"

using namespace Lib;

#include "functional"

using std::function;

class DialogAnswer : public Window
{
private:
	string text;

	function<void(Window* owner)> callback_ok;
	function<void(Window* owner)> callback_cancel;

public:
	DialogAnswer(string title, SimpleRect size, string text, function<void(Window* owner)> callback_ok = [](Window* owner) {},
		function<void(Window* owner)> callback_cancel = [](Window* owner) {});

public:
	void setup();

};