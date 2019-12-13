#include "kit-main.h"

Lib::Kit* Lib::Kit::instance = nullptr;

Lib::Kit* Lib::Kit::init()
{
	if (instance == nullptr)
		instance = new Kit;

	return instance;
}

Lib::Kit::Kit()
{
	setlocale(LC_ALL, "ru_RU.UTF-8");
	setlocale(LC_NUMERIC, "eng");

	this->is_running = true;


	this->setup();
}

Lib::Kit::~Kit()
{
	if (instance != nullptr)
		delete instance;
}

void Lib::Kit::run()
{
	render();
	onEvent();

	close();
}

Lib::Window* Lib::Kit::at(size_t index)
{
	if (index >= windows.size())
		return nullptr;

	return windows[index];
}

Lib::Window* Lib::Kit::operator[](size_t index)
{
	return at(index);
}

Lib::Window* Lib::Kit::addWindow(Window* window)
{
	window->parent = this;
	windows.push_back(window);

	render();

	return windows.back();
}

void Lib::Kit::setup()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		cout << "ERROR: SDL could not initialize! SDL Error: %s\n" << SDL_GetError();
		return;
	}


	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
	SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");
}

void Lib::Kit::render()
{
	for (auto& window : windows)
	{
		window->render();
	}
}

void Lib::Kit::onEvent()
{
	int windowId = -1;
	while (is_running && SDL_WaitEvent(&e))
	{
		windowId = e.window.windowID - 1;

		if (windowId < windows.size())
			windows[windowId]->onEvent(&e);
	}
}

void Lib::Kit::close()
{
	is_running = false;
}
