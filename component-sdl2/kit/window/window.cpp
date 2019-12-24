#include "window.h"
#include "../kit-main.h"

void Kit::Window::handleStyles()
{
	for (auto& [id, component] : allComponents)
	{
		CSS::css* component_css = component->getComponentStyles();
		if (component_css == nullptr)
			continue;

		for (auto& [block_id, css_block] : component_css->getStyles())
		{
			allComponentsStyles[block_id] = css_block;
		}
	}

	for (auto& [id, component] : allComponents)
	{
		auto classnames = Utils::split(component->_classes, ' ');

		for (auto& classname : *classnames)
		{
			if (allComponentsStyles.find(classname) != allComponentsStyles.end())
			{
				auto style = allComponentsStyles[classname];

				component->_cssBlock.mergeWith(style);
			}
		}

		delete classnames;
	}
}

Kit::Component* Kit::Window::addElement(Component* component)
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

Kit::Component* Kit::Window::getElementById(string id) const
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

Kit::Components Kit::Window::getElementsByClassName(string className) const
{
	vector<Component*>* ñomponentVector = new vector<Component*>;
	for (auto& [id, ñomponent] : allComponents)
	{
		if (ñomponent->hasClass(className))
		{
			ñomponentVector->push_back(ñomponent);
		}
	}

	return Components(ñomponentVector);
}

Kit::Component& Kit::Window::add(string id, string classes, vector<Component*> childrens)
{
	Component* _addComponent = new Component(id, classes, childrens);

	return add(_addComponent);
}

Kit::Component& Kit::Window::add(Component* component)
{
	navigator->append(component);

	return *component;
}

Kit::Component* Kit::Window::create(string id, string classes, vector<Component*> childrens)
{
	return new Component(id, classes, childrens);
}

Kit::Component* Kit::Window::create(Component* component)
{
	return component;
}

CSS::css_block* Kit::Window::addStyle(string className, CSS::css_block style)
{
	allComponentsStyles.insert(make_pair(className, style));
	return &allComponentsStyles[className];
}

Kit::Window::Window(string title, SimpleRect size)
{
	this->title = title;
	this->_size = size;
	this->_id = -1;

	this->window = nullptr;
	this->renderer = nullptr;

	this->is_display = true;

	this->wasSetupStyle = false;
	this->wasSetupComponents = false;

	this->need_close = false;

	this->init();
}

Kit::Window::~Window()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	for (auto& [id, component] : allComponents)
	{
		delete component;
	}
}

void Kit::Window::init()
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

	this->_id = SDL_GetWindowID(window);

	preSetup();
}

void Kit::Window::preSetup()
{
	navigator = $$ = new Navigator(this);
	Window::addElement(navigator);
}

void Kit::Window::render()
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

void Kit::Window::onEvent(Event* e)
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

void Kit::Window::onAnimate()
{
	navigator->animate();
}

void Kit::Window::show()
{
	render();

	is_display = true;

	SDL_ShowWindow(window);
}

void Kit::Window::hide()
{

	is_display = false;

	SDL_HideWindow(window);
}

bool Kit::Window::isShow()
{
	return is_display;
}

void Kit::Window::collapse()
{
	SDL_MinimizeWindow(window);
}

void Kit::Window::close()
{
	parent->deleteWindow(this->_id);
}

size_t Kit::Window::id()
{
	return _id;
}

void Kit::Window::include(string path)
{
	main_css.open(path);
	Window::allComponentsStyles = main_css.getStyles();
}

int Kit::Window::width() const
{
	return _size.w;
}

int Kit::Window::height() const
{
	return _size.h;
}

int Kit::Window::top() const
{
	return _size.y;
}

int Kit::Window::left() const
{
	return _size.x;
}

Kit::SimpleRect Kit::Window::size() const
{
	return _size;
}

SDL_Renderer* Kit::Window::getRenderer() const
{
	return renderer;
}

SDL_Window* Kit::Window::getWindow() const
{
	return window;
}

void Kit::Window::setDraggableArea(SimpleRect area_)
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

void Kit::Window::mouseButtonDown(SDL_Event* e)
{
	Component* hover = navigator->onComponentHover({ e->motion.x, e->motion.y });

	if (hover != nullptr)
	{
		hover->mouseButtonDown(e);
	}


}

void Kit::Window::mouseButtonUp(SDL_Event* e)
{
	Component* hover = navigator->onComponentHover({ e->motion.x, e->motion.y });

	if (hover != nullptr)
	{
		hover->mouseButtonUp(e);
	}

	
}

void Kit::Window::mouseMotion(SDL_Event* e)
{
	Component* hover = navigator->onComponentHover({ e->motion.x, e->motion.y });

	if (hover != nullptr)
	{
		hover->mouseMotion(e);
	}

	
}

void Kit::Window::mouseWheel(SDL_Event* e)
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


}

void Kit::Window::keyDown(SDL_Event* e)
{
}

void Kit::Window::keyUp(SDL_Event* e)
{
}
