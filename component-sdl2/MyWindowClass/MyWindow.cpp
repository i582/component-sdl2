#include "MyWindow.h"

MyWindow::MyWindow(string title, SimpleRect size)
	: Window(title, size)
{
	setup();
}

void MyWindow::setup()
{
	include("./css/style.css");

	/*$$->append(
		new Component("main", { "45px", "30px", "100% - 65px", "100% - 50px" }, ".main", 
		{
			new Component("#left-side", { "0px", "30px", "45px", "100% - 50px" }, ".left-side", 
			{
				new Component("#settings", { "8px", "100% - 50px", "30px", "30px" }, ".settings")
			})
	}));*/

	$$->append(new Button("#button", { 250, 250, 70, 25 }, ".button .button-blue", "Текст"));

	$$->append(new Button("#button1", { 330, 250, 70, 25 }, ".button", "Текст"));

	$$->append(new MyComponent("#test", { 0, 0, 200, 200 }, ""));
	$$->append(new MyComponent("#test1", { 200, 0, 200, 200 }, ""));

	/*Window::getElementById("#settings")->addEventListener("click", [&](Component* sender, Event* e)
	{
		cout << "Settings click" << endl;

	});*/
}
