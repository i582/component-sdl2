#pragma once

#include "../../kit/component/component.h"

using namespace Kit;

class FileDialogItem : public Component
{
	string text;

public:
	FileDialogItem(string id, Rect size, string classes, string text);


public:
	void setup();
};