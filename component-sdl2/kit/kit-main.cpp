#include "kit-main.h"

Kit::KitApplication* Kit::KitApplication::instance = nullptr;

Kit::KitApplication* Kit::KitApplication::create()
{
	if (instance == nullptr)
		instance = new KitApplication;

	return instance;
}

Kit::KitApplication::KitApplication()
{
	setlocale(LC_ALL, "ru_RU.UTF-8");
	setlocale(LC_NUMERIC, "eng");

	this->is_running = true;

	this->count_deleted_windows = 0;

	init();
}

Kit::KitApplication::~KitApplication()
{
	for (auto& window : windows)
	{
		delete window;
	}

	Font::close();

	delete instance;
}

int Kit::KitApplication::run()
{
	if (setup() == -1)
	{
		return -1;
	}

	render();
	onEvent();

	close();
}

Kit::Window* Kit::KitApplication::at(size_t index)
{
	if (index >= windows.size())
		return nullptr;

	return windows[index];
}

Kit::Window* Kit::KitApplication::operator[](size_t index)
{
	return at(index);
}

Kit::vector<Kit::Window*>* Kit::KitApplication::getWindows()
{
	return &windows;
}

Kit::Window* Kit::KitApplication::addWindow(Window* window)
{
	window->parent = this;
	windows.push_back(window);

	render();

	return windows.back();
}

void Kit::KitApplication::init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		cout << "ERROR: SDL could not initialize! SDL Error: %s\n" << SDL_GetError();
		return;
	}
}

int Kit::KitApplication::setup()
{
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
	SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");

	return 0;
}

void Kit::KitApplication::render()
{
	for (auto& window : windows)
	{
		window->render();
	}
}

void Kit::KitApplication::onEvent()
{
	int windowId = -1;
	while (is_running && SDL_WaitEvent(&e))
	{
		windowId = e.window.windowID - 1;

		if (windowId != 0)
			windowId -= count_deleted_windows;

		if (windowId < windows.size())
			windows[windowId]->onEvent(&e);
	}
}

void Kit::KitApplication::close()
{
	is_running = false;

	this->~KitApplication();
}
