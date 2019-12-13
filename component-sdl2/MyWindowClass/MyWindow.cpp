#include "MyWindow.h"
#include "../kit/kit-main.h"

#include "../DialogWindow/DialogWindow.h"

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
		})
	);*/

	setDraggableArea({ 0, 0, _size.w - 135, 25 });

	$$->append(new Button("#button", { 250, 250, 70, 25 }, ".button .button-blue", "Текст"));

	$$->append(new Button("#button1", { 330, 250, 70, 25 }, ".button", "Текст"));

	$$->append(new MyComponent("#test", { 0, 0, 200, 200 }, ""));
	$$->append(new MyComponent("#test1", { 200, 0, 200, 200 }, ""));



	$$->append(new SystemButton("#system-exit", { "100% - 45px", "0px", "45px", "30px" }, ".system-exit"));
	$$->append(new SystemButton("#system-expand", { "100% - 90px", "0px", "45px", "30px" }, ".system-expand"));
	$$->append(new SystemButton("#system-collapse", { "100% - 135px", "0px", "45px", "30px" }, ".system-collapse"));

	
	Window::getElementById("#system-exit")->addEventListener("onmouseup", [&](Component* sender, Event* e)
	{
		parent->addWindow(new DialogAnswer("Предупреждение", { -1, -1, 200, 150 }, "Вы действительно хотите выйти из системы?",
		[&](Window* owner)
		{
			owner->close();
			close();
		},
		[&](Window* owner)
		{
			owner->close();
		})
		);
		
	});

	Window::getElementById("#system-collapse")->addEventListener("onmouseup", [&](Component* sender, Event* e)
	{
		collapse();
	});

	Window::getElementById("#button")->addEventListener("onmouseup", [&](Component* sender, Event* e)
	{
		parent->addWindow(new DialogWindow("", { 300, 300, 560, 95 }));
	});
}
