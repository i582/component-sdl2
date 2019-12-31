#include "window.h"

#include <utility>
#include <SDL_syswm.h>
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

		auto is_have_styles = false;
		CSS::css_block ready_block(id, true);

		for (auto& classname : *classnames)
		{
			if (allComponentsStyles.find(classname) != allComponentsStyles.end())
			{
				auto style = allComponentsStyles[classname];
                ready_block.mergeWith(style);
				component->_cssBlock.mergeWith(ready_block);

                is_have_styles = true;
			}
		}

		if (!is_have_styles)
        {
            component->_cssBlock.mergeWith(ready_block);
        }

		delete classnames;
	}
}

Kit::Component* Kit::Window::addElement(Component* component)
{
	if (component == nullptr)
		return nullptr;

	const string& objectID = component->id();

	if (allComponents.find(objectID) != allComponents.end())
	{
		throw std::logic_error("Component with id: " + objectID + " already exists");
	}
	else
	{
		allComponents.insert(make_pair(objectID, component));
	}

	return allComponents[objectID];
}

Kit::Component* Kit::Window::getElementById(const string& id) const
{
	Component* component = allComponents.at(id);

	if (component == nullptr)
	{
        throw std::logic_error("Component with id: " + id + " not found!");
	}
	else
	{
		return component;
	}
}

Kit::Components Kit::Window::getElementsByClassName(const string& className) const
{
	auto* componentVector = new vector<Component*>;

	for (auto& [id, component] : allComponents)
	{
		if (component->hasClass(className))
		{
			componentVector->push_back(component);
		}
	}

	return Components(componentVector);
}

Kit::Component& Kit::Window::add(const string& id, const string& classes, const vector<Component*>& childrens)
{
	auto* _addComponent = new Component(id, classes, childrens);

	return add(_addComponent);
}

Kit::Component& Kit::Window::add(const std::string& classes, const std::vector<Kit::Component*>& childrens)
{
    auto* _addComponent = new Component("", classes, childrens);

    return add(_addComponent);
}

Kit::Component& Kit::Window::add(Component* component)
{
	navigator->append(component);

	return *component;
}

Kit::Component* Kit::Window::create(const string& id, const string& classes, const vector<Component*>& childrens)
{
	return new Component(id, classes, childrens);
}

Kit::Component* Kit::Window::create(const std::string& classes, const std::vector<Kit::Component*>& childrens)
{
    return new Component("", classes, childrens);
}

Kit::Component* Kit::Window::create(Component* component)
{
	return component;
}

CSS::css_block* Kit::Window::addStyle(const string& className, CSS::css_block style)
{
	allComponentsStyles.insert(make_pair(className, style));
	return &allComponentsStyles[className];
}

Kit::Window::Window(const string& title, SimpleRect size)
{
	this->title = title;
	this->_size = size;
	this->_id = -1;

	this->window = nullptr;
	this->renderer = nullptr;

	this->is_display = true;

	this->wasSetupStyle = false;
	this->wasSetupComponents = false;

	this->focusComponent = nullptr;

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
	    const string error = "Error of initialize new Window: ";
        throw std::logic_error(error + SDL_GetError());
	}

	this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr)
	{
        const string error = "Renderer could not be created! SDL Error: ";
        throw std::logic_error(error + SDL_GetError());
	}

	this->_id = SDL_GetWindowID(window);

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

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

	SDL_SetRenderTarget(renderer, nullptr);

	SDL_SetRenderDrawColor(renderer,  0x00, 0x00, 0x00, 0x00 );
	SDL_RenderFillRect(renderer, nullptr);

	SDL_SetRenderDrawColor(renderer,  0xff, 0xff, 0xff, 0x00 );
	SDL_RenderDrawRect(renderer, nullptr);



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

	case SDL_WINDOWEVENT:
	{
		switch (e->window.event)
		{
		case SDL_WINDOWEVENT_MAXIMIZED:
		case SDL_WINDOWEVENT_RESTORED:
		{
			render();
			break;
		}
		}

		break;
	}



	default:break;

		
	}
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

void Kit::Window::include(const string& path)
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

void Kit::Window::setDraggableArea(SimpleRect _area)
{
	auto size = new SimpleRect;
	*size = _area;

	auto rects = new vector<SimpleRect>;

    rects->push_back(_size);
    rects->push_back(_area);

    SDL_SetWindowHitTest(window, [](SDL_Window* win, const SDL_Point* area, void* callback_data) -> SDL_HitTestResult
	{
        auto rects = (vector<SimpleRect>*)callback_data;

        SimpleRect rect = rects->at(1);
        SimpleRect size = rects->at(0);

//		SimpleRect rect = *(SimpleRect*)callback_data;
//
//		SDL_Rect rec = { 0, 0, rect.w, rect.h };


        SimpleRect resizeBottom = { 10, size.h - 10, size.w, 10 };


		if (SDL_PointInRect(area, &rect))
			return SDL_HITTEST_DRAGGABLE;

		if (SDL_PointInRect(area, &resizeBottom))
		    return SDL_HITTEST_RESIZE_BOTTOM;

		return SDL_HITTEST_NORMAL;
	}
	, rects);

}

void Kit::Window::mouseButtonDown(SDL_Event* e)
{
    Component* hover = navigator->onComponentHover({ e->motion.x, e->motion.y });

    if (focusComponent != nullptr)
    {
        if (focusComponent != hover)
        {
            focusComponent->loseFocus(e);
            focusComponent = hover;
            focusComponent->getFocus(e);
        }
    }
    else
    {
        focusComponent = hover;
        focusComponent->getFocus(e);
    }

    focusComponent->mouseButtonDown(e);
}

void Kit::Window::mouseButtonUp(SDL_Event* e)
{
    if (focusComponent != nullptr)
    {
        focusComponent->mouseButtonUp(e);
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
