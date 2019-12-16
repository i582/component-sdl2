#include "window.h"
#include "../kit-main.h"

void Lib::Window::handleStyles()
{
	for (auto& component : allComponents)
	{
		CSS::css* component_css = component.second->getComponentStyles();
		if (component_css == nullptr)
			continue;

		for (auto& css_block : component_css->getStyles())
		{
			allComponentsStyles[css_block.first] = css_block.second;
		}
	}


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

	this->need_close = false;

	this->init();
}

Lib::Window::~Window()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	for (auto& element : allComponents)
	{
		delete element.second;
	}


}

void Lib::Window::init()
{
	this->window = SDL_CreateWindow(title.c_str(),
		_size.x == -1 ? SDL_WINDOWPOS_CENTERED : _size.x,
		_size.y == -1 ? SDL_WINDOWPOS_CENTERED : _size.y,
		_size.w, _size.h, SDL_WINDOW_BORDERLESS); //SDL_WINDOW_RESIZABLE

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

	SDL_SetRenderDrawColor(renderer,  0x00, 0x00, 0x00, 0xff );
	SDL_RenderFillRect(renderer, NULL);

	SDL_SetRenderDrawColor(renderer,  0xff, 0xff, 0xff, 0x00 );
	SDL_RenderDrawRect(renderer, NULL);



	navigator->render();


	SDL_RenderPresent(renderer);
}

void Lib::Window::onEvent(Event* e)
{
	if (need_close)
	{
		Window* temp = parent->at(1);
		parent->count_deleted_windows++;
		parent->getWindows()->erase(parent->getWindows()->end() - 1);

		delete temp;

		Component::_hoverComponent = nullptr;
		return;
	}

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

		if (need_close)
		{
			Window* temp = parent->at(1);
			parent->count_deleted_windows++;
			parent->getWindows()->erase(parent->getWindows()->end() - 1);

			delete temp;

			Component::_hoverComponent = nullptr;
			return;
		}
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

	/*case SDL_WINDOWEVENT:
	{
		switch (e->window.event) {
		case SDL_WINDOWEVENT_MINIMIZED:
		case SDL_WINDOWEVENT_MAXIMIZED:
		case SDL_WINDOWEVENT_RESTORED:
		case SDL_WINDOWEVENT_ENTER:
		case SDL_WINDOWEVENT_LEAVE:
		case SDL_WINDOWEVENT_SHOWN:
		{
			render();
			break;
		}
		}

		break;
	}*/

	break;

	default:
	{
		
		break;
	}
		
	}
}

void Lib::Window::show()
{
	render();

	is_display = true;

	SDL_ShowWindow(window);
}

void Lib::Window::hide()
{

	is_display = false;

	SDL_HideWindow(window);
}

bool Lib::Window::isShow()
{
	return is_display;
}

void Lib::Window::collapse()
{
	SDL_MinimizeWindow(window);
}

void Lib::Window::close()
{
	this->need_close = true;
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

void Lib::Window::setDraggableArea(SimpleRect area_)
{
	SimpleRect* size = new SimpleRect;
	*size = area_;

	SDL_SetWindowHitTest(window, [](SDL_Window* win, const SDL_Point* area, void* callback_data) -> SDL_HitTestResult
	{
		SimpleRect rect = *(SimpleRect*)callback_data;

		SDL_Rect rec = { 0, 0, rect.w, rect.h };


		if (SDL_PointInRect(area, &rec))
			return SDL_HITTEST_DRAGGABLE;

		return SDL_HITTEST_NORMAL;
	}
	, size);

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
