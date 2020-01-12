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
        window* addWindow(window* window);

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
        window* at(size_t index);
        window* operator[](size_t index);

        void terminate();

    private:
        bool _isRunning;
        Event _e;

        map<size_t, window*> _windows;

    private: /** Friends */
        friend window;


    private:
        void init();

        void setupWindows();

        void render();
        void onEvent();


    };

}	
