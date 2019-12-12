#include "DialogWindow.h"

DialogWindow::DialogWindow(string title, SimpleRect size)
	: Window(title, size)
{
	setup();
}

/** 560 / 95 */
void DialogWindow::setup()
{
	include("./css/dialog-window.css");


	$$->append(new Component("#header", { "0px", "0px", "100%", "25px" }, ".header"));

	$$->append(new Button("#button-submit", { 475, 50, 70, 25 }, ".button", "Îê"));
}
