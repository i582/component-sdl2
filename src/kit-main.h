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
        static KitApplication* getInstance();
        ~KitApplication();


    public:
        /**
         * @brief Adds a window to the application.
         * @param window Pointer to the added window
         * @return Pointer to the added window
         */
        Window* addWindow(Window* window);

        /**
         * @brief Deletes a window by its index (the index is set automatically at creation)
         */
        void deleteWindow(size_t index);


    public: /** Interface */

        /**
         * @brief The main function for starting the library should be called after calls to addWindow
         * or others in the return statement (return $ .run ()).
         */
        int run();


    public: /** Windows Interface */
        Window* at(size_t index);
        Window* operator[](size_t index);


    private:
        bool is_running;
        Event e;

        map<size_t, Window*> windows;

    private: /** Friends */
        friend Window;


    private:
        void init();
        void render();
        void onEvent();
        void close();

    };

}	
