#include "FileDialogItem.h"

FileDialogItem::FileDialogItem(string id, Rect size, string classes, string text)
	: Component(id, size, classes)
{
	this->text = text;
	setup();
}

void FileDialogItem::setup()
{
	include("./FileDialog/FileDialogItem/css/file-dialog-item.css");

	append(new Component(_id + "#main-container-item", { "0px", "0px", "100%", "20px" }, ".main-container-item"));

	auto item = _childrens[0];

	item->append(new Component(_id + "#image-arrow", { "5px", "5px", "10px", "10px" }, ".image-arrow-left"))
		->addEventListener("onmousedown", [](Component* sender, Event* e)
		{
			sender->toggleClass(".image-arrow-left");
			sender->toggleClass(".image-arrow-down");
		});

	item->append(new Component(_id + "#image-folder", { "24px", "3px", "15px", "15px" }, ".image-folder"));

	
	item->setText(this->text);
}
