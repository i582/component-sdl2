#pragma once

#include "SDL.h"
#include "SDL_syswm.h"
#include "iostream"
#include "string"
#include "exception"

#include "../tools/rect/simple-rect/simple-rect.h"
#include "../event/event.h"

#include "../component/navigator/navigator.h"
#include "../component/components/components.h"

#include "Windows.h"

namespace Kit
{
    using std::cout;
    using std::endl;
    using std::string;

    class KitApplication;

    class Window
    {

    public: /** Component Interface */

        /**
         * @brief Returns the component with the given identifier.
         */
        [[nodiscard]] Component* getElementById(const string& id) const;

        /**
         * @brief Returns components with the given class identifier.
         */
        [[nodiscard]] Components getElementsByClassName(const string& className) const;


        /**
         * @brief Functions for adding components to the window.
         */
        Component* add(const string& id, const string& classes, const vector<Component*>& childrens = {});
        Component* add(const string& classes = "", const vector<Component*>& childrens = {});
        Component* add(Component* component);


    public: /** constructor & destructor*/
        Window(const string& title, const SimpleRect& size, bool noBorder = false);
        ~Window();


    public: /** Interface */
        void render();
        void onEvent(Event* e);

        void show();
        void hide();
        bool isShow();

        void collapse();
        void close();

        size_t id();


    public: /** Size Interface */

        [[nodiscard]] int width() const;
        [[nodiscard]] int height() const;
        [[nodiscard]] int top() const;
        [[nodiscard]] int left() const;
        [[nodiscard]] SimpleRect size() const;


    public: /** SDL Interface */

        [[nodiscard]] SDL_Renderer* getRenderer() const;
        [[nodiscard]] SDL_Window* getWindow() const;


    public: /** Other Interface */

        /**
         * @brief Sets the area beyond which the window can be moved.
         */
        void setDraggableArea(SimpleRect area);


    public: /** CSS Interface */

        /**
         *  @brief Function for include css style file
         */
        void style(const string& path);


    public: /** Component Style Interface */

        CSS::css_block* addStyle(const string& className, const CSS::css_block& style);

    protected:
        map<string, Component*> allComponents;
        map<string, CSS::css_block> allComponentsStyles;

    protected:
        Component* focusComponent;

    protected:
        SimpleRect _size;
        string title;
        size_t _id;

        SDL_Window* window;
        SDL_Renderer* renderer;

        bool isDisplay;

        bool noBorder;

        Navigator* navigator;
        Navigator* $$;

        CSS::css mainCSS;


        bool wasSetupStyle;
        bool wasSetupComponents;


        KitApplication* parent;


        bool isMainWindow;

    protected:
        friend Component;
        friend KitApplication;

    protected:
        void init();

        void preSetup();

        virtual void setup(){};

    protected:
        /**
         * @brief A function that processes all elements and adds styles
         * associated with classes for each component
         */
        void handleStyles();

        /**
         * @brief Adds a component to the window.
         * If a component with such an identifier exists, an exception (logic_error) is thrown.
         */
        Component* addElement(Component* component);


    protected: /** Events */
        void mouseButtonDown(Event* e);
        void mouseButtonUp(Event* e);
        void mouseMotion(Event* e);
        void mouseWheel(Event* e);
        void keyDown(Event* e);
        void keyUp(Event* e);
        void textInput(Event* e);
    };

}