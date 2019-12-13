#include "DialogAnswer.h"

DialogAnswer::DialogAnswer(string title, SimpleRect size, string text, 
	function<void(Window* owner)> callback_ok, function<void(Window* owner)> callback_cancel)
	: Window(title, size)
{
	this->callback_ok = callback_ok;
	this->callback_cancel = callback_cancel;
	this->text = text;
	setup();
}

/** 300 / 150 */
void DialogAnswer::setup()
{
	include("./css/dialog-window.css");

	$$->append(new Component("#header", { "0px", "0px", "100%", "25px" }, ".header"))
		->setText(title);

	$$->append(new Button("#button-submit", { "75% - 35px", "100% - 35px", "75px", "25px" }, ".button .button-blue", "Ок"));
	$$->append(new Button("#button-close", { "25% - 35px", "100% - 35px", "75px", "25px" }, ".button", "Отмена"));

	
	$$->append(new Component("#warning-image", { "10px", "50% - 35px", "45px", "45px" }, ".warning-image"));


	$$->append(new Component("#text-info", { "65px", "35px", "100% - 75px", "100% - 70px" }, ".text-info"))
		->setText(this->text);


	Window::getElementById("#button-submit")->addEventListener("onmouseup", [&](Component* sender, Event* e)
	{
		callback_ok(this);
	});

	Window::getElementById("#button-close")->addEventListener("onmouseup", [&](Component* sender, Event* e)
	{
		callback_cancel(this);
	});


	setDraggableArea({ 0, 0, _size.w, 25 });
}
