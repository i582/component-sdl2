#include "FileDialogItems.h"

FileDialogItems::FileDialogItems(string id, Rect size, string classes, const vector<vector<string>>& items)
	: Component(id, size, classes)
{
	this->items = items;
	setup();
}

void FileDialogItems::setup()
{
	include("./FileDialog/FileDialogItems/css/file-dialog-items.css");


	for (size_t i = 0; i < items.size(); i++)
	{
		int item_y = i * 100;

		append(new FileDialogItem(_id + "#item-items" + to_string(i), { "0px", to_string(item_y) + "px", "100%", "20px", }, "", items[i][0]));

		if (items.size() > 1)
		{
			items[i].erase(items[i].begin());
			append(new FileDialogItems(_id + "#items" + to_string(i), { "10px", "20px", "100% - 10px", "80px", }, "", { items[i] }));
		}
			
	}
}
