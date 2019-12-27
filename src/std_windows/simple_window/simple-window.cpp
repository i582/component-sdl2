#include "simple-window.h"
#include "kit-main.h"


SimpleWindow::SimpleWindow(string title, SimpleRect size)
	: Window(title, size)
{
	setup();
}

void SimpleWindow::setup()
{
	include("../src/std_windows/simple_window/css/style.css");
	setDraggableArea({ 0, 0, _size.w - 135, 25 });


	$$->append(new Component("#window-header", { 0, 0, _size.w - 135, 30 }, ".window-header"))
		->setText(this->title);

	$$->append(new Component("#system-exit", { "100% - 45px", "0px", "45px", "30px" }, ".system-button .system-exit"));
	$$->append(new Component("#system-expand", { "100% - 90px", "0px", "45px", "30px" }, ".system-button .system-expand"));
	$$->append(new Component("#system-collapse", { "100% - 135px", "0px", "45px", "30px" }, ".system-button .system-collapse"));


    $$->append(new Component("#label", { "100px", "100px", "100px", "30px" }, ".label"))
    ->setText("Hello World!");


    add(Button::create("#button-1", "Ok", ".button-ok .button-blue"));
    add(Button::create("#button-cancel", "Cancel", ".button-cancel"));


    add(Checkbox::create("#checkbox", "First item", ".first-checkbox"));


    add(Select::create("#select-1", "Choose item", ".select-1"));



    Window::getElementById("#checkbox")->addEventListener("change", [](Component* sender, Event* e)
    {
        cout << "Состояние изменено!" << endl;
    });

//
//	add("#test-block", ".test-class",
//	{
//		create("#test-block-inner", ".test-class-inner-1",
//		{
//			create("#test-block-inner-inner", ".test-class-inner-inner")
//		}),
//		create("#test-block-inner-2", ".test-class-inner-2")
//	});



}
