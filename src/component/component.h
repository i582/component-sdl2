#pragma once

#include "SDL2_gfxPrimitives.h"

#include "../tools/rect/extended-rect/extended-rect.h"
#include "../tools/size/simple-size/simple-size.h"
#include "tools/image/image.h"
#include "../tools/font/font.h"
#include "../tools/text/text.h"
#include "../tools/css/css.h"
#include "scroll/vertical-scroll/vertical_scroll.h"
#include "scroll/horizontal-scroll/horizontal_scroll.h"

#include "../tools/text2/text.h"
#include "component-header.h"

#include "../tools/draw/draw.h"

namespace Kit
{
    using CSS::Color;

    class Window;

    class window;

    class Component
    {

    public:

        /**
         * @brief A set of functions for creating class objects.
         */
        static Component* create(const string& id, const string& classes, const vector<Component*>& childrens = {});
        static Component* create(const string& classes = "", const vector<Component*>& childrens = {});
        static Component* create(Component* component);


    public: /** Constructors */
        explicit Component(const string& id, const string& classes = "", const vector<Component*>& childrens = {});

        explicit Component(const string& id = "", const Rect& size = {0, 0, 0, 0}, const string& classes = "",
                           const vector<Component*>& childrens = {});

        virtual ~Component();


    public: /** Size Interface */

        [[nodiscard]]  int width() const;
        [[nodiscard]] int height() const;
        [[nodiscard]] int top() const;
        [[nodiscard]] int left() const;

        [[nodiscard]] const Rect& size() const;
        [[nodiscard]] const Rect& outerSize() const;
        [[nodiscard]] const Rect& innerSize() const;

        void outerWidth(int value);
        void outerHeight(int value);
        void outerTop(int value);
        void outerLeft(int value);

    public: /** Display Interface */

        Component* show();
        Component* hide();
        Component* toggleDisplay();
        [[nodiscard]] bool display() const;


    public: /** Render Data Interface */

        [[nodiscard]] SDL_Renderer* renderer() const;
        [[nodiscard]] SDL_Texture* innerTexture() const;
        [[nodiscard]] SDL_Texture* outerTexture() const;


    public: /** Ralation Interface */

        [[nodiscard]] Component* parent() const;

        [[nodiscard]] const vector<Component*>& childrens() const;

        [[nodiscard]] window* parentWindow() const;

        /**
         * @brief Checks if the passed object is a child of this
         * @return true|false
         */
        bool isChildrenObject(Component* obj) const;

        /**
         * @brief Checks if the passed object is a parent of this
         * @return true|false
         */
        bool isParentObject(Component* obj) const;


    public: /** Append Interface */

        /**
         * @brief Adds a child component to the component passed as the first parameter.
         * @param component Pointer to added component
         * @return Pointer to added component
         */
        virtual Component* append(Component* component);


        /**
         * @brief Adds a component collection to a component as a collection of child components.
         * @return Self pointer
         */
        Component* append(const vector<Component*>& components);



    public: /** Identifiers Interface */

        [[nodiscard]] const string& id() const;


    public: /** Event listeners Interface */

        /**
         * @brief Associates the passed callback function with the event with the passed
         * identifier. When an event occurs with the transmitted identifier,
         * the callback function will be called
         *
         * @param action - Event id
         * @param eventCallback - Functor with form is "function <void (Component* sender, Event* e)>"
         */
        Component* addEventListener(const string& action, eventCallback callback_function);


        /**
         * @brief Deletes the callback function for the event with the passed identifier
         *
         * @param action - Event id
         */
        Component* removeEventListener(const string& action);


        /**
         * @brief Immediately calls the callback function associated with the event
         * with the passed identifier
         *
         * @param action - Event id
         * @param e - Event data
         */
        Component* callEventListener(const string& action, Event* e);


    public: /** User Data Interface */

        map<string, std::any>& userData();
        Component* addUserData(const string& key, const std::any& data);
        std::any userData(const string& key);


    public: /** Class Interface */

        [[nodiscard]] bool hasClass(const string& className) const;
        Component* removeClass(const string& className);
        Component* addClass(const string& className);
        Component* toggleClass(const string& className);
        const string& classes();
        Component* classes(const string& newClasses);


    public: /** Text Interface */
        virtual Component* setText(const string& text);


    public: /** Style Interface */
        Component* style(const string& path);
        Component* style(const map<string, string>& inlineStyles);

    public: /** Focus Interface */
        Component* getFocus(Event* e);
        Component* loseFocus(Event* e);


    public: /** Ignore Event Interface */
        Component* ignoreEvents();
        Component* noIgnoreEvents();
        bool isIgnoreEvents();


    public: /** Extended Text Interface */
        Component* useExtendedText();
        Component* unuseExtendedText();



    protected:
        /** For Event */
        static Component* _hoverComponent;

    protected:
        /** Sizes */
        Rect _innerSize;
        Rect _outerSize;
        bool _isConstructSize;

        bool _autoWidth;
        bool _autoHeight;

        /** Identifiers */
        string _id;
        string _classes;


        /** Parents */
        Component* _parent;
        window* _window;


        /** Childrens */
        vector<Component*> _childrens;
        Rect _childrensSize;


        /** Display */
        bool _isDisplay;


        /** Events Listener */
        map<string, eventCallback> _eventListeners;

        static void _emptyCallback(Component* sender, Event* e){};


        /** User Data */
        map<string, std::any> _userData;


        /** State */
        bool _isHovered;
        bool _isActive;
        bool _isFocused;

        /** SDL */
        SDL_Renderer* _renderer;
        SDL_Texture* _innerTexture;
        SDL_Texture* _outerTexture;


        /** Styles */
        CSS::css_block _cssBlock;


        /** Other for Events */
        bool _isEnterInComponent;


        /** Vertical Scroll part */
        vertical_scroll* _verticalScroll;
        bool _verticalScrollable;
        bool _isVerticalScrollActive;


        /** Horizontal Scroll part */
        horizontal_scroll* _horizontalScroll;
        bool _horizontalScrollable;
        bool _isHorizontalScrollActive;

        bool _needRenderScroll;


        /** Text part */
        font _fontNormal;
        font _fontHover;
        font _fontActive;
        font _fontFocus;

        Text* _text;
        string _text_temp;


        /** Image */
        image* _image;


        /** CSS component */
        CSS::css* _css_component;


        /** Extended text */
        Text2* _extended_text;
        bool _withExtendedText;


        /** Ignore Some Event */
        bool _ignoreEvents;


        /** Inline Styles */
        map<string, string> _inlineStyles;


    private:
        friend Window;
        friend class window;

    protected:


        void render();

        /**
         * Setup event listeners
         */
        void setupEventListeners();

        /**
         * Helper relationship function
         * @return The first parent for the current one that needs vertical scrolling
         */
        Component* getFirstVerticalScrollableParent();

        /**
         * Helper relationship function
         * @return The first parent for the current one that needs horizontal scrolling
         */
        Component* getFirstHorizontalScrollableParent();


        /**
         * @brief Function for getting component styles that were connected directly in it
         * @return Pointer to css styles
         */
        CSS::css* getComponentStyles();


        /**
         * @brief Function for generating a random string for an identifier
         * @return Random string
         */
        static string generateRandomString();


    protected: /** Setup Functions */

        /**
         * @brief Configures component dimensions using either sizes from styles or
         * dimensions specified directly in the constructor
         */
        void setupSize();

        /**
         * @brief Sets all descendants to a common renderer starting from the
         * component whose function was called. Usually called on the
         * main component (Navigator) of a window
         */
        void setupChildrenRenderer();


        /**
         * @brief Calculates the dimensions of an element if their sizes are
         * specified as a percentage or as an expression
         */
        void computeSize();


        /**
         * @brief Calculates the size of the rectangle into which all children can
         * fit, if the size of this rectangle is greater than the height of the
         * inner one, a vertical scroll is added
         */
        void computeChildrenSize();


        /**
         * @brief Configures images inside a component
         */
        void setupBackgroundImage();

        /**
         * @brief Configures font inside a component
         */
        void setupFont();

        /**
         * @brief Configures text inside a component
         */
        void setupText();

        /**
         * @brief Configures scrolls inside a component
         */
        void setupScrolls();

        /**
         * @brief Setting the parent window for elements when adding directly
         * through the 4 parameter of the Component constructor
         */
        void setupParentWindow();


        /**
         * @brief Depending on the current value of "display", sets the positions
         * of children
         */
        void setupChildrenPosition();


        /**
         * @brief Configures extended text inside a component
         */
        void setupExtendedText();


        /**
         * @brief Sets the generated identifier if the identifier is an empty string
         */
        void checkID();


        /**
         * @brief The function corrects the coordinates of the mouse
         * relative to the element in which it is now located
         *
         * @param p Mouse point
         */
        void adjustMousePoint(Point& p);


    protected: /** Setup */

        /**
         * @brief Function to configure the container and all its childrens
         */
        void setupComponents();


    protected: /** Events */

        void mouseButtonDown(Event* e);
        void mouseButtonUp(Event* e);
        void mouseMotion(Event* e);
        void mouseOut(Event* e);
        void mouseScroll(Event* e, int scrollDirection);
        void keyDown(Event* e);
        void textInput(Event* e);


    protected: /** Hover */

        /**
         * @brief Checks if a given point is internal to an external size
         * @return true|false
         */
        bool onHover(const Point& point);

        /**
         * @brief It passes through all the children and finds in which component
         * the transmitted point is located. Moreover, if the point is in the component,
         * and at the same time in this component it is in the child, then the child
         * is returned. That is, the function goes as deep as possible until there
         * is an unambiguous component in which the point is now
         */
        Component* onComponentHover(Point point);


    protected: /** Scroll */
        [[nodiscard]] bool isVerticalScrollable() const;
        [[nodiscard]] bool isHorizontalScrollable() const;


    };


}