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



	$$->append(
		new Component("#test-scroll", { 450, 30, 200, 300 }, ".outer-block", 
		{
			new Component("#test-scroll-inner", { 0, 0, 200, 400 }, ".inner-block",
			{
				new Button("#button-inner", { 20, 20, 70, 25 }, ".button", "Ок")
			}),
			new Component("#test-scroll-inner-2", { 0, 450, 200, 200 }, ".inner-block")
		})
	);

	Window::getElementById("#test-scroll-inner-2")->addEventListener("click", [&](Component* sender, Event* e)
	{
		cout << "Inner-2 click" << endl;
	});

	Window::getElementById("#test-scroll-inner")->addEventListener("click", [&](Component* sender, Event* e)
	{
		cout << "Inner-1 click" << endl;
	});


	$$->append(new Button("#button", { 250, 250, 70, 25 }, ".button .button-blue", "Текст"));

	$$->append(new Button("#button1", { 330, 250, 70, 25 }, ".button", "Текст"));

	$$->append(new MyComponent("#test", { 0, 30, 200, 200 }, ""));
	$$->append(new MyComponent("#test1", { 200, 30, 200, 200 }, ""));


	$$->append(new Checkbox("#checkbox", { 400, 400, 100, 16 }, ".checkbox", "Текст", 
		[](Component* sender, Event* e)
		{
			if (((Checkbox*)sender)->isChecked())
			{
				cout << "Checkbox установлен!" << endl;
			}
			else
			{
				cout << "Checkbox снят!" << endl;
			}
		})
	);

	$$->append(new RadioButton("#radio", { 400, 420, 100, 16 }, ".radio-button", "group-1", "Текст",
		[](Component* sender, Event* e)
		{
			if (((Checkbox*)sender)->isChecked())
			{
				cout << "Radio установлен!" << endl;
			}
			else
			{
				cout << "Radio снят!" << endl;
			}
		})
	);
	


	$$->append(new Component("#window-header", { 0, 0, _size.w - 135, 30 }, ".window-header"))
		->setText("Заголовок");

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
