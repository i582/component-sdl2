#pragma once

#include "../kit/component/component.h"

#include "FileDialogItems/FileDialogItems.h"

using namespace Kit;


class folders
{
public:
	string text;

	vector<folders> folders_vector;

	folders();
	folders(string text);

	void add(const folders& folder);
};


class FileDialog : public Component
{
public:
	FileDialog(string id, Rect size, string classes);
	

public:
	void setup();
};