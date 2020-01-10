#include "kit-main.h"

Kit::KitApplication* Kit::KitApplication::instance = nullptr;

Kit::KitApplication* Kit::KitApplication::getInstance()
{
    if (instance == nullptr)
        instance = new KitApplication;

    return instance;
}

Kit::KitApplication::KitApplication()
{
    this->_isRunning = true;
    this->_e = {0};

    this->init();
}

Kit::KitApplication::~KitApplication()
{
    for (auto&[id, window] : _windows)
    {
        delete window;
    }

    font::delete_cache();
    font_find::delete_cache();
    image::delete_cache();

    delete instance;
}

int Kit::KitApplication::run()
{
    render();
    onEvent();

    return 0;
}

Kit::window* Kit::KitApplication::at(size_t index)
{
    if (_windows.find(index) == _windows.end())
        return nullptr;

    return _windows[index];
}

Kit::window* Kit::KitApplication::operator[](size_t index)
{
    return at(index);
}

Kit::window* Kit::KitApplication::addWindow(window* window)
{
    window->_parent = this;

    if (window->_id == 1)
    {
        window->_isMainWindow = true;
    }

    _windows.insert(std::make_pair(window->_id, window));

    render();

    return _windows[window->_id];
}

void Kit::KitApplication::deleteWindow(size_t index)
{
    if (_windows.find(index) == _windows.end())
        return;

    _windows.erase(index);
}

void Kit::KitApplication::init()
{
    setlocale(LC_ALL, "ru_RU.UTF-8");
    setlocale(LC_NUMERIC, "eng");

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        cout << "ERROR: SDL could not initialize! SDL Error: %s\n" << SDL_GetError();
        return;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");
}

void Kit::KitApplication::render()
{
    for (auto&[id, window] : _windows)
    {
        window->render();
    }
}

void Kit::KitApplication::onEvent()
{
    SDL_StartTextInput();
    while (_isRunning && SDL_WaitEvent(&_e) && !_windows.empty())
    {
        const int windowId = _e.window.windowID;
        if (_windows.find(windowId) != _windows.end())
        {
            _windows[windowId]->onEvent(&_e);
            _windows[windowId]->render();
        }
    }
    SDL_StopTextInput();
}

void Kit::KitApplication::terminate()
{
    _isRunning = false;
}
