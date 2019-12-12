#include "window.h"

Lib::Window::Window(string title, SimpleRect size)
{
	this->title = title;
	this->size = size;

	this->window = nullptr;
	this->renderer = nullptr;

	this->is_display = true;

	this->init();
}

Lib::Window::~Window()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}

void Lib::Window::init()
{
	this->window = SDL_CreateWindow(title.c_str(),
		size.x == -1 ? SDL_WINDOWPOS_CENTERED : size.x,
		size.y == -1 ? SDL_WINDOWPOS_CENTERED : size.y,
		size.w, size.h, SDL_WINDOW_RESIZABLE);

	if (window == nullptr)
	{
		cout << "Error of initialize new Window: " << SDL_GetError() << endl;
		return;
	}

	this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr)
	{
		cout << "Renderer could not be created! SDL Error: %s\n" << SDL_GetError();
		return;
	}

	preSetup();
}

void Lib::Window::preSetup()
{
}

void Lib::Window::show()
{
	is_display = true;

	SDL_ShowWindow(window);
}

void Lib::Window::hide()
{
	is_display = false;

	SDL_ShowWindow(window);
}

bool Lib::Window::isShow()
{
	return is_display;
}

void Lib::Window::close()
{
	SDL_DestroyWindow(window);
}
