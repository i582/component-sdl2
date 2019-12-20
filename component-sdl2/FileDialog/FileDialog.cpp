#include "FileDialog.h"

FileDialog::FileDialog(string id, Rect size, string classes)
	: Component(id, size, classes)
{
	setup();
}

void FileDialog::setup()
{
	include("./FileDialog/css/file-dialog.css");

	append(new Component("#main-container", { "1px", "1px", "100% - 20px", "100% - 2px" }, ".main-container"));


	const vector<string> folder = {
		"C:\\",
		"D:\\",
		"E:\\"
	};


	const vector< vector<string> > folders = {
		{"C:\\", "windows"},
		{"D:\\", "media"},
		{"E:\\", "photo"}
	};

	const vector<string> folder_two = {
		"window",
		"media",
		"photo"
	};


	auto cont = _childrens[0];

	const int height = folder.size() * 20;

	cont->append(new FileDialogItems(_id + "#items", { "0px", "0px", "100%", to_string(height) + "px", }, "", folders));

	/*for (size_t i = 0; i < 1; i++)
	{
		int item_y = i * 20;

		cont->append(new FileDialogItem("#item" + to_string(i), { "0px", to_string(item_y) + "px", "100%", "100px", }, ""));
	}*/



}

folders::folders() : folders("") {}

folders::folders(string text)
{
	this->text = text;
}

void folders::add(const folders& folder)
{
	folders_vector.push_back(folder);
}
