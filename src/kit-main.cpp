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
    this->is_running = true;
    this->e = {0};

    this->init();
}

Kit::KitApplication::~KitApplication()
{
    for (auto&[id, window] : windows)
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
    if (windows.find(index) == windows.end())
        return nullptr;

    return windows[index];
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

    windows.insert(std::make_pair(window->_id, window));

    render();

    return windows[window->_id];
}

void Kit::KitApplication::deleteWindow(size_t index)
{
    if (windows.find(index) == windows.end())
        return;

    windows.erase(index);
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
    for (auto&[id, window] : windows)
    {
        window->render();
    }
}

void Kit::KitApplication::onEvent()
{
    SDL_StartTextInput();
    while (is_running && SDL_WaitEvent(&e) && !windows.empty())
    {
        const int windowId = e.window.windowID;
        if (windows.find(windowId) != windows.end())
        {
            windows[windowId]->onEvent(&e);
            windows[windowId]->render();
        }
    }
    SDL_StopTextInput();
}

void Kit::KitApplication::terminate()
{
    is_running = false;
}
