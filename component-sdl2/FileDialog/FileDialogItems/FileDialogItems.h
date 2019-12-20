#pragma once

#include "../../kit/component/component.h"
#include "../FileDialogItem/FileDialogItem.h"
using namespace Kit;

class FileDialogItems : public Component
{
private:
	vector<vector<string>> items;

public:
	FileDialogItems(string id, Rect size, string classes, const vector<vector<string>>& items);


public:
	void setup();
};