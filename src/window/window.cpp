#include <SDL_syswm.h>
#include "window.h"
#include "../kit-main.h"

Kit::window::window(const string& title_, const SimpleRect& size_, bool noBorder_)
{
    this->_focusComponent = nullptr;

    this->_window = nullptr;
    this->_renderer = nullptr;


    this->_size = size_;
    this->_title = title_;
    this->_id = -1;


    this->_isDisplay = false;
    this->_noBorder = noBorder_;
    this->_isMainWindow = false;

    this->_wasSetupStyle = false;
    this->_wasSetupComponents = false;

    this->_isMaximize = false;

    this->_navigator = nullptr;


    this->_parent = nullptr;



    this->init();
}

Kit::window::~window()
{
    SDL_DestroyWindow(_window);
    SDL_DestroyRenderer(_renderer);
}

Kit::Component* Kit::window::getElementById(const std::string& id_) const
{
    if (_components.find(id_) == _components.end())
    {
        throw std::logic_error("Component with id: " + id_ + " not found!");
    }

    return _components.at(id_);
}

Kit::Components Kit::window::getElementsByClassName(const std::string& className_) const
{
    auto componentVector = new vector<Component*>;

    for (auto&[id, component] : _components)
    {
        if (component->hasClass(className_))
        {
            componentVector->push_back(component);
        }
    }

    return Components(componentVector);
}

Kit::Component* Kit::window::add(const std::string& id_, const std::string& classes_,
                                 const std::vector<Kit::Component*>& childrens_)
{
    auto newComponent = new Component(id_, classes_, childrens_);

    return add(newComponent);
}

Kit::Component* Kit::window::add(const std::string& classes_, const std::vector<Kit::Component*>& childrens_)
{
    auto newComponent = new Component("", classes_, childrens_);

    return add(newComponent);
}

Kit::Component* Kit::window::add(Kit::Component* component_)
{
    _navigator->append(component_);

    return component_;
}

void Kit::window::render()
{
    SDL_SetRenderTarget(_renderer, nullptr);
    SDL_SetRenderDrawColor(_renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(_renderer);


    _navigator->render();

    if (_isDisplay)
    {
        SDL_RenderPresent(_renderer);
    }

    if (!_isDisplay)
    {
        _navigator->render();

        SDL_RenderPresent(_renderer);

        _isDisplay = true;
        SDL_ShowWindow(_window);
    }
}

void Kit::window::show()
{
    _isDisplay = true;

    render();

    SDL_ShowWindow(_window);
}

void Kit::window::hide()
{
    _isDisplay = false;

    SDL_HideWindow(_window);
}

bool Kit::window::isShow() const
{
    return _isDisplay;
}

void Kit::window::collapse() const
{
    SDL_MinimizeWindow(_window);
}

void Kit::window::maximize() const
{
    if (_isMaximize)
    {
        SDL_RestoreWindow(_window);
        _isMaximize = false;
    }
    else
    {
        SDL_MaximizeWindow(_window);
        _isMaximize = true;
    }
}

void Kit::window::close() const
{
    SDL_Event closeEvent;
    closeEvent.type = SDL_WINDOWEVENT;
    closeEvent.window.event = SDL_WINDOWEVENT_CLOSE;
    closeEvent.window.windowID = _id;

    SDL_PushEvent(&closeEvent);
}

void Kit::window::raise() const
{
    SDL_RaiseWindow(_window);
}

size_t Kit::window::id() const
{
    return _id;
}

int Kit::window::width() const
{
    return _size.w;
}

int Kit::window::height() const
{
    return _size.h;
}

int Kit::window::top() const
{
    return _size.y;
}

int Kit::window::left() const
{
    return _size.x;
}

Kit::SimpleRect Kit::window::size() const
{
    return _size;
}

SDL_Renderer* Kit::window::renderer() const
{
    return _renderer;
}

SDL_Window* Kit::window::sdlWindow() const
{
    return _window;
}

void Kit::window::style(const std::string& path_)
{
    _styles.open(path_);
    _componentsStyles = _styles.getStyles();
}

void Kit::window::setDraggableArea(const Kit::SimpleRect& area_)
{
    // TODO: Recode

    auto size = new SimpleRect;
    *size = area_;

    auto rects = new vector<SimpleRect>;

    rects->push_back(_size);
    rects->push_back(area_);

    SDL_SetWindowHitTest(_window, [](SDL_Window* win, const SDL_Point* area, void* callback_data) -> SDL_HitTestResult
    {
        auto rects = (vector <SimpleRect>*) callback_data;

        SimpleRect rect = rects->at(1);
        SimpleRect size = rects->at(0);

		//SimpleRect rect = *(SimpleRect*)callback_data;

		SDL_Rect rec = { 0, 0, rect.w, rect.h };


        SimpleRect resizeBottom = {10, size.h - 2, size.w, 10};


        if (SDL_PointInRect(area, &rect))
            return SDL_HITTEST_DRAGGABLE;

        //if (SDL_PointInRect(area, &resizeBottom))
        //    return SDL_HITTEST_RESIZE_BOTTOM;

        return SDL_HITTEST_NORMAL;
    }, rects);

}

void Kit::window::addEventListener(const std::string& action_, Kit::windowEventCallback callback_)
{
    _eventListeners[action_] = std::move(callback_);
}

void Kit::window::removeEventListener(const std::string& action_)
{
    _eventListeners[action_] = window::_emptyCallback;
}


/** Protected Further */

void Kit::window::init()
{
    _window = SDL_CreateWindow(_title.c_str(),
                               _size.x == -1 ? SDL_WINDOWPOS_CENTERED : _size.x,
                               _size.y == -1 ? SDL_WINDOWPOS_CENTERED : _size.y,
                               _size.w, _size.h, SDL_WINDOW_BORDERLESS); //SDL_WINDOW_RESIZABLE

    if (_window == nullptr)
    {
        const string error = "Error of initialize new Window: ";
        throw std::logic_error(error + SDL_GetError());
    }

    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
    if (_renderer == nullptr)
    {
        const string error = "Renderer could not be created! SDL Error: ";
        throw std::logic_error(error + SDL_GetError());
    }
    SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);


    SDL_GetWindowPosition(_window, &_size.x, &_size.y);

    _id = SDL_GetWindowID(_window);


    preset();
}

void Kit::window::preset()
{
    _navigator = new Navigator(this);
    addElement(_navigator);

    setupEventListeners();
}

void Kit::window::setupEventListeners()
{
    _eventListeners["shown"] = window::_emptyCallback;
    _eventListeners["hidden"] = window::_emptyCallback;
    _eventListeners["exposed"] = window::_emptyCallback;

    _eventListeners["moved"] = window::_emptyCallback;

    _eventListeners["resized"] = window::_emptyCallback;

    _eventListeners["minimized"] = window::_emptyCallback;
    _eventListeners["maximized"] = window::_emptyCallback;
    _eventListeners["restored"] = window::_emptyCallback;

    _eventListeners["enter"] = window::_emptyCallback;
    _eventListeners["leave"] = window::_emptyCallback;
    _eventListeners["focus_gained"] = window::_emptyCallback;
    _eventListeners["focus_lost"] = window::_emptyCallback;
    _eventListeners["close"] = window::_emptyCallback;
    _eventListeners["take_focus"] = window::_emptyCallback;
    _eventListeners["hit_test"] = window::_emptyCallback;
}

void Kit::window::finalSetup()
{
    handleStyles();

    _navigator->setupComponents();
}

void Kit::window::handleStyles()
{
    //_navigator->handleStyles();

    for (auto&[id, component] : _components)
    {
        CSS::css* component_css = component->getComponentStyles();
        if (component_css == nullptr)
            continue;

        for (auto&[block_id, css_block] : component_css->getStyles())
        {
            _componentsStyles[block_id] = css_block;
        }
    }


    for (auto&[id, component] : _components)
    {
        auto classnames = Utils::split(component->_classes, ' ');

        auto is_have_styles = false;
        CSS::css_block ready_block(id, true);

        for (auto& classname : *classnames)
        {
            if (_componentsStyles.find(classname) != _componentsStyles.end())
            {
                auto style = _componentsStyles[classname];
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

Kit::Component* Kit::window::addElement(Kit::Component* component_)
{
    if (component_ == nullptr)
        return nullptr;

    const string& objectID = component_->id();

    if (_components.find(objectID) != _components.end())
    {
        throw std::logic_error("Component with id: " + objectID + " already exists");
    }
    else
    {
        _components.insert(make_pair(objectID, component_));
    }

    return _components[objectID];
}

void Kit::window::onEvent(Kit::Event* e_)
{
    switch (e_->type)
    {

        case SDL_MOUSEMOTION:
        {
            mouseMotion(e_);
            break;
        }

        case SDL_MOUSEBUTTONDOWN:
        {
            mouseButtonDown(e_);
            break;
        }

        case SDL_MOUSEBUTTONUP:
        {
            mouseButtonUp(e_);
            break;
        }

        case SDL_MOUSEWHEEL:
        {
            mouseWheel(e_);
            break;
        }

        case SDL_KEYDOWN:
        {
            keyDown(e_);
            break;
        }

        case SDL_TEXTINPUT:
        {
            textInput(e_);
            break;
        }

        case SDL_WINDOWEVENT:
        {
            switch (e_->window.event)
            {
                case SDL_WINDOWEVENT_SHOWN:
                {
                    _eventListeners["shown"](this, e_);

                    break;
                }

                case SDL_WINDOWEVENT_HIDDEN:
                {
                    _eventListeners["hidden"](this, e_);
                    return;
                    break;
                }

                case SDL_WINDOWEVENT_EXPOSED:
                {
                    _eventListeners["exposed"](this, e_);
                    return;
                    break;
                }

                case SDL_WINDOWEVENT_MOVED:
                {
                    _size.x = e_->window.data1;
                    _size.y = e_->window.data2;

                    _eventListeners["moved"](this, e_);
                    return;
                    break;
                }

                case SDL_WINDOWEVENT_RESIZED:
                {
                    _eventListeners["resized"](this, e_);
                    break;
                }

                case SDL_WINDOWEVENT_MINIMIZED:
                {
                    _eventListeners["minimized"](this, e_);
                    break;
                }

                case SDL_WINDOWEVENT_MAXIMIZED:
                {
                    _eventListeners["maximized"](this, e_);
                    break;
                }

                case SDL_WINDOWEVENT_RESTORED:
                {
                    _eventListeners["restored"](this, e_);
                    break;
                }

                case SDL_WINDOWEVENT_ENTER:
                {
                    _eventListeners["enter"](this, e_);
                    return;
                    break;
                }

                case SDL_WINDOWEVENT_LEAVE:
                {
                    _eventListeners["leave"](this, e_);
                    return;
                    break;
                }

                case SDL_WINDOWEVENT_FOCUS_GAINED:
                {
                    _eventListeners["focus_gained"](this, e_);
                    return;
                    break;
                }

                case SDL_WINDOWEVENT_FOCUS_LOST:
                {
                    _eventListeners["focus_lost"](this, e_);
                    return;
                    break;
                }

                case SDL_WINDOWEVENT_CLOSE:
                {
                    _eventListeners["close"](this, e_);

                    if (_isMainWindow)
                    {
                        _parent->terminate();
                    }
                    else
                    {
                        SDL_DestroyWindow(_window);
                        SDL_DestroyRenderer(_renderer);
                    }
                    return;
                    break;
                }

                case SDL_WINDOWEVENT_TAKE_FOCUS:
                {
                    _eventListeners["take_focus"](this, e_);
                    return;
                    break;
                }

                case SDL_WINDOWEVENT_HIT_TEST:
                {
                    _eventListeners["hit_test"](this, e_);
                    return;
                    break;
                }

                default: return;
            }

            break;
        }


        default: return;
    }

    render();
}


void Kit::window::mouseButtonDown(SDL_Event* e_)
{
    Component* hover = _navigator->onComponentHover({e_->motion.x, e_->motion.y});

    if (_focusComponent != nullptr)
    {
        if (_focusComponent != hover)
        {
            _focusComponent->loseFocus(e_);
            _focusComponent = hover;
            _focusComponent->getFocus(e_);
        }
    }
    else
    {
        _focusComponent = hover;
        _focusComponent->getFocus(e_);
    }

    _focusComponent->mouseButtonDown(e_);
}

void Kit::window::mouseButtonUp(SDL_Event* e_)
{
    if (_focusComponent != nullptr)
    {
        _focusComponent->mouseButtonUp(e_);
    }
}

void Kit::window::mouseMotion(SDL_Event* e_)
{
    if (_focusComponent != nullptr)
    {
        if (_focusComponent->_isHorizontalScrollActive || _focusComponent->_isVerticalScrollActive)
        {
            _focusComponent->mouseMotion(e_);
            return;
        }
    }

    Component* hover = _navigator->onComponentHover({e_->motion.x, e_->motion.y});

    if (hover != nullptr)
    {
        hover->mouseMotion(e_);
    }
}

void Kit::window::mouseWheel(SDL_Event* e_)
{
    const int scrollDirection = e_->wheel.y;

    if (e_->motion.x == -1 || e_->motion.y == -1 || e_->motion.x == 1 || e_->motion.y == 1)
    {
        SDL_GetMouseState(&e_->motion.x, &e_->motion.y);
    }

    Component* hover = _navigator->onComponentHover({e_->motion.x, e_->motion.y});

    if (hover != nullptr)
    {
        hover->mouseScroll(e_, scrollDirection);
    }
}

void Kit::window::textInput(SDL_Event* e_)
{
    if (_focusComponent != nullptr)
    {
        _focusComponent->textInput(e_);
    }
}

void Kit::window::keyDown(SDL_Event* e_)
{
    if (_focusComponent != nullptr)
    {
        _focusComponent->keyDown(e_);
    }
}

Kit::KitApplication* Kit::window::parent()
{
    return _parent;
}

int Kit::window::topBorderSize() const
{
    int top = 0;
    SDL_GetWindowBordersSize(_window, &top, nullptr, nullptr, nullptr);
    return top;
}
