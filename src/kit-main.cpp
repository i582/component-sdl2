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

    init();
}

Kit::KitApplication::~KitApplication()
{
	for (auto& [id, window] : windows)
	{
		delete window;
	}

	font::clear_cache();
	font_find::delete_cache();

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
	if (windows.find(index) == windows.end())
		return nullptr;

	return windows[index];
}

Kit::Window* Kit::KitApplication::operator[](size_t index)
{
	return at(index);
}

Kit::Window* Kit::KitApplication::addWindow(Window* window)
{
	window->parent = this;

	windows.insert(std::make_pair(window->id(), window));

	render();

	return windows[window->id()];
}

void Kit::KitApplication::deleteWindow(size_t index)
{
	if (windows.find(index) == windows.end())
		return;

	windows.erase(index);
}

void Kit::KitApplication::init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		cout << "ERROR: SDL could not initialize! SDL Error: %s\n" << SDL_GetError();
		return;
	}

	anim = new animation<int>(0, 100, 100, nullptr, []() {});
}

int Kit::KitApplication::setup()
{
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");

	return 0;
}

void Kit::KitApplication::render()
{
	for (auto& [id, window] : windows)
	{
		window->render();
	}
}

void Kit::KitApplication::onEvent()
{
	while (is_running && SDL_WaitEvent(&e) && !windows.empty())
	{
        const int windowId = e.window.windowID;
        if (windows.find(windowId) != windows.end())
        {
            windows[windowId]->onEvent(&e);
            windows[windowId]->render();
        }
	}
}

void Kit::KitApplication::close()
{
	is_running = false;
}
