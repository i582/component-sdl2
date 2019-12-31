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




    add(Button::create("#button-1", "Ok", ".button-ok .button-blue"));
    add(Button::create("#button-cancel", "Cancel", ".button-cancel"));

    add(Checkbox::create("#checkbox", "select this", ".first-checkbox"));



	add(new Table("#table", ".table-test"));

    auto table = (Table*)Window::getElementById("#table");

    table->addCollumn("#");
    table->addCollumn("ФИО");
    table->addCollumn("Русский язык");
    table->addCollumn("Математика");

    table->addRow({ "1", "Молодой человек", "85", "90" });
    table->addRow({ "2", "Старый человек", "75", "100" });
    table->addRow({ "3", "Старый человек", "75", "100" });

}
