#pragma once

#include "window/window.h"
#include "event/event.h"

namespace Kit
{
    using std::cout;
    using std::endl;


    class KitApplication
    {
    private:
        static KitApplication* instance;

        KitApplication();

    public:
        static KitApplication* create();

        ~KitApplication();


    private:
        bool is_running;
        Event e;

        map<size_t, Window*> windows;

    public:
        Window* addWindow(Window* window);

        void deleteWindow(size_t index);

        friend Window;

    private:
        void init();
        void render();
        void onEvent();
        void close();

    public: /** Interface */
        int run();


    public: /** Windows Interface */
        Window* at(size_t index);
        Window* operator[](size_t index);

    };

}	
