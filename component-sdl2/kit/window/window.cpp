#include "window.h"

void Lib::Window::handleStyles()
{

	for (auto& style : allComponentsStyles)
	{
		string className = style.first;

		for (auto& component : allComponents)
		{
			if (component.second->hasClass(className))
			{
				component.second->_cssBlock.mergeWith(style.second);
			}
		}

	}
}

Lib::Component* Lib::Window::addElement(Component* component)
{
	if (component == nullptr)
		return nullptr;

	string objectID = component->id();

	if (allComponents.find(objectID) != allComponents.end())
	{
		cout << "Ýëåìåíò ñ id = " << objectID << " óæå ñóùåñòâóåò!" << endl;
		return nullptr;
	}
	else
	{
		allComponents.insert(make_pair(objectID, component));
	}

	return allComponents[objectID];
}

Lib::Component* Lib::Window::getElementById(string id) const
{
	Component* component = allComponents.at(id);
	if (component == nullptr)
	{
		cout << "Ýëåìåíò ñ id = " << id << " íå ñóùåñòâóåò!" << endl;
		return nullptr;
	}
	else
	{
		return component;
	}
}

Lib::Components Lib::Window::getElementsByClassName(string className) const
{
	vector<Component*>* ñomponentVector = new vector<Component*>;
	for (auto& ñomponent : allComponents)
	{
		if (ñomponent.second->hasClass(className))
		{
			ñomponentVector->push_back(ñomponent.second);
		}
	}

	return Components(ñomponentVector);
}

CSS::css_block* Lib::Window::addStyle(string className, CSS::css_block style)
{
	allComponentsStyles.insert(make_pair(className, style));
	return &allComponentsStyles[className];
}

Lib::Window::Window(string title, SimpleRect size)
{
	this->title = title;
	this->_size = size;

	this->window = nullptr;
	this->renderer = nullptr;

	this->is_display = true;

	this->wasSetupStyle = false;
	this->wasSetupComponents = false;

	this->init();
}

Lib::Window::~Window()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	delete $$;
}

void Lib::Window::init()
{
	this->window = SDL_CreateWindow(title.c_str(),
		_size.x == -1 ? SDL_WINDOWPOS_CENTERED : _size.x,
		_size.y == -1 ? SDL_WINDOWPOS_CENTERED : _size.y,
		_size.w, _size.h, 0); //SDL_WINDOW_RESIZABLE

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
	navigator = $$ = new Navigator(this);
	Window::addElement(navigator);
}

void Lib::Window::render()
{
	if (!wasSetupStyle)
	{
		Window::handleStyles();
		wasSetupStyle = true;
	}

	if (!wasSetupComponents)
	{
		navigator->setupComponents();
		wasSetupComponents = true;
	}

	SDL_SetRenderTarget(renderer, NULL);

	SDL_SetRenderDrawColor(renderer,  0x33, 0x33, 0x33, 0xff );
	SDL_RenderFillRect(renderer, NULL);

	SDL_SetRenderDrawColor(renderer,  0xff, 0xff, 0xff, 0x00 );
	SDL_RenderDrawRect(renderer, NULL);



	navigator->render();


	SDL_RenderPresent(renderer);
}

void Lib::Window::onEvent(Event* e)
{
	switch (e->type)
	{

	case SDL_MOUSEMOTION:
	{
		mouseMotion(e);
		break;
	}

	case SDL_MOUSEBUTTONDOWN:
	{
		mouseButtonDown(e);
		break;
	}

	case SDL_MOUSEBUTTONUP:
	{
		mouseButtonUp(e);
		break;
	}

	case SDL_MOUSEWHEEL:
	{
		mouseWheel(e);
		break;
	}

	case SDL_KEYDOWN:
	{
		keyDown(e);
		break;
	}

	case SDL_KEYUP:
	{
		keyUp(e);
		break;
	}

	break;
	}
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

void Lib::Window::include(string path)
{
	main_css.open(path);
	Window::allComponentsStyles = main_css.getStyles();
}

int Lib::Window::width() const
{
	return _size.w;
}

int Lib::Window::height() const
{
	return _size.h;
}

int Lib::Window::top() const
{
	return _size.y;
}

int Lib::Window::left() const
{
	return _size.x;
}

Lib::SimpleRect Lib::Window::size() const
{
	return _size;
}

SDL_Renderer* Lib::Window::getRenderer() const
{
	return renderer;
}

SDL_Window* Lib::Window::getWindow() const
{
	return window;
}

void Lib::Window::mouseButtonDown(SDL_Event* e)
{
	Component* hover = navigator->onComponentHover({ e->motion.x, e->motion.y });

	if (hover != nullptr)
	{
		hover->mouseButtonDown(e);
	}

	render();
}

void Lib::Window::mouseButtonUp(SDL_Event* e)
{
	Component* hover = navigator->onComponentHover({ e->motion.x, e->motion.y });

	if (hover != nullptr)
	{
		hover->mouseButtonUp(e);
	}

	render();
}

void Lib::Window::mouseMotion(SDL_Event* e)
{
	Component* hover = navigator->onComponentHover({ e->motion.x, e->motion.y });

	if (hover != nullptr)
	{
		hover->mouseMotion(e);
	}

	render();
}

void Lib::Window::mouseWheel(SDL_Event* e)
{
	const int scrollDirection = e->wheel.y;

	if (e->motion.x == -1 || e->motion.y == -1 || e->motion.x == 1 || e->motion.y == 1)
	{
		SDL_GetMouseState(&e->motion.x, &e->motion.y);
	}

	Component* hover = navigator->onComponentHover({ e->motion.x, e->motion.y });

	if (hover != nullptr)
	{
		hover->mouseScroll(e, scrollDirection);
	}

	render();
}

void Lib::Window::keyDown(SDL_Event* e)
{
}

void Lib::Window::keyUp(SDL_Event* e)
{
}
