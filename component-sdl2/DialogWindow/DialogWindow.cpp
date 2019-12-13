#include "DialogWindow.h"
#include "../kit/kit-main.h"

DialogWindow::DialogWindow(string title, SimpleRect size)
	: Window(title, size)
{
	setup();
}

/** 560 / 95 */
void DialogWindow::setup()
{
	

	include("./css/dialog-window.css");

	SDL_SetWindowBordered(window, SDL_FALSE);

	$$->append(new Component("#header", { "0px", "0px", "100%", "25px" }, ".header"));

	$$->append(new Button("#button-submit", { 475, 50, 70, 25 }, ".button", "Ок"));






	Window::getElementById("#button-submit")->addEventListener("onmouseup", [&](Component* sender, Event* e)
	{
		

		this->close();
	});

	//parent->addWindow(nullptr);

	setDraggableArea({ 0, 0, _size.w, 25 });
}
