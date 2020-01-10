#pragma once

#include <string>
#include <SDL.h>

#include "../component/components/components.h"
#include "../component/navigator/navigator.h"


namespace Kit
{
    using std::string;
    using CSS::css;


    using windowEventCallback = function<void(window* sender, Event* e_)>;

    class KitApplication;

    class window
    {
    public: // Constructor & Destructor
        window(const string& title_, const SimpleRect& size_, bool noBorder_ = false);
        ~window();

    public: // Component Interface

        /**
         * @brief Returns the component with the given identifier.
         * If a component with the specified identifier is not found, an exception is thrown
         */
        [[nodiscard]] Component* getElementById(const string& id_) const;

        /**
         * @brief Returns components with the given class identifier.
         */
        [[nodiscard]] Components getElementsByClassName(const string& className_) const;


        /**
        * @brief Functions for adding components to the window.
        */
        Component* add(const string& id_, const string& classes_, const vector<Component*>& childrens_ = {});
        Component* add(const string& classes_ = "", const vector<Component*>& childrens_ = {});
        Component* add(Component* component_);



    public: // Render Interface
        void render();


    public: // Display Interface
        void show();
        void hide();
        [[nodiscard]] bool isShow() const;


    public: // System Event Interface

        /**
         * @brief Minimize a window to an iconic representation.
         */
        void collapse() const;

        /**
         * @brief Make a window as large as possible or restore
         * the size and position of a minimized or maximized window.
         */
        void maximize() const;

        /**
         * @brief Closes the window. If the window is first created in the
         * application, then the application terminates.
         */
        void close() const;

        /**
         * @brief Raise a window above other windows and set the input focus.
         */
        void raise() const;



    public: // ID Interface
        [[nodiscard]] size_t id() const;


    public: // Size Interface
        [[nodiscard]] int width() const;
        [[nodiscard]] int height() const;
        [[nodiscard]] int top() const;
        [[nodiscard]] int left() const;
        [[nodiscard]] SimpleRect size() const;

        [[nodiscard]] int topBorderSize() const;

    public: // SDL Interface
        [[nodiscard]] SDL_Renderer* renderer() const;
        [[nodiscard]] SDL_Window* sdlWindow() const;


    public: // CSS Interface

        /**
         *  @brief Function for include css style file
         */
        void style(const string& path_);


    public: // Other Interface

        /**
         * @brief Sets the area beyond which the window can be moved.
         */
        void setDraggableArea(const SimpleRect& area_);


        KitApplication* parent();

    public: // Event Listeners Interface
        void addEventListener(const string& action_, windowEventCallback callback_);

        void removeEventListener(const string& action_);

    protected: // Components Part
        map<string, Component*> _components;
        map<string, CSS::css_block> _componentsStyles;

    protected:
        Component* _focusComponent;


    protected: // SDL Part
        SDL_Window* _window;
        SDL_Renderer* _renderer;

    protected:
        SimpleRect _size;
        string _title;
        size_t _id;


    protected: // States
        bool _isDisplay;
        bool _noBorder;
        bool _isMainWindow;

        bool _wasSetupStyle;
        bool _wasSetupComponents;

        mutable bool _isMaximize;


        map<string, windowEventCallback> _eventListeners;

        static void _emptyCallback(window* sender, Event* e){};

    protected:
        Navigator* _navigator;

    protected: // Style Part
        css _styles;

    protected: // Parent
        KitApplication* _parent;


    protected: // Friend
        friend Component;
        friend KitApplication;


    protected:
        void init();
        void preset();
        void setupEventListeners();
        virtual void setup() {};

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
        Component* addElement(Component* component_);

    protected:
        void onEvent(Event* e_);

    protected: /** Events */
        void mouseButtonDown(Event* e_);
        void mouseButtonUp(Event* e_);
        void mouseMotion(Event* e_);
        void mouseWheel(Event* e_);
        void textInput(Event* e_);
        void keyDown(Event* e_);


    };

}