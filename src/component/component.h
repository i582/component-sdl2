#pragma once

#include "SDL2_gfxPrimitives.h"

#include "../tools/rect/extended-rect/extended-rect.h"
#include "../tools/size/simple-size/simple-size.h"
#include "../tools/image/image.h"
#include "../tools/font/font.h"
#include "../tools/text/text.h"
#include "../tools/css/css.h"
#include "scroll/vertical-scroll/vertical-scroll.h"
#include "scroll/horizontal-scroll/horizontal-scroll.h"

#include "animation/animation.h"
#include "../tools/text2/text.h"
#include "component-header.h"

#include "../tools/draw/draw.h"

namespace Kit
{
using CSS::Color;

class Window;

using Cmpt = Component;

class Component
{

public:
    static Component* create(const string& id, const string& classes, const vector<Component*>& childrens = {});
    static Component* create(const string& classes = "", const vector<Component*>& childrens = {});
    static Component* create(Component* component);

private:
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
	Window* _window;


	/** Childrens */
	vector<Component*> _childrens;
	Rect _childrensSize;


	/** Display */
	bool _isDisplay;


	/** Events Listener */
	map <string, eventCallback> _eventListeners;
	static void _emptyCallback(Component* sender, Event* e) {};


	/** User Data */
	map <string, std::any> _userData;


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
	VerticalScroll* _verticalScroll;
	bool _verticalScrollable;
	bool _isVerticalScrollActive;


	/** Horizontal Scroll part */
	HorizontalScroll* _horizontalScroll;
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
	Image* _image;


	/** CSS component */
	CSS::css* _css_component;


	/** Extended text */
    Text2* _extended_text;
    bool _withExtendedText;

    /** Ignore Some Event */
    bool _ignoreEvents;

public:
    explicit Component(const string& id, const string& classes = "", const vector<Component*>& childrens = {});

    explicit Component(const string& id = "", const Rect& size = { 0, 0, 0, 0 }, const string& classes = "", const vector<Component*>& childrens = {});

	virtual ~Component();


public:
	friend Window;

protected:

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

protected: /** State */

    bool isHovered();
    bool isActive();


protected: /** Scroll */
    bool isVerticalScrollable() const;
    bool isHorizontalScrollable() const;


public: /** Render Interface */

	void render();


public: /** Size Interface */
	
	int width() const;
	int height() const;
	int top() const;
	int left() const;
	const Rect& size() const;
	const Rect& outerSize() const;
	const Rect& innerSize() const;

    void outerWidth(int value);
    void outerHeight(int value);
    void outerTop(int value);
    void outerLeft(int value);

public: /** Display Interface */

	Component* show();
	Component* hide();
	Component* toggleDisplay();
	bool display() const;


public: /** Render Data Interface */

	SDL_Renderer* renderer() const;
	SDL_Texture* innerTexture() const;
	SDL_Texture* outerTexture() const;


public: /** Ralation Interface */

	Component* parent() const;
	const vector <Component*>& childs() const;
	Window* window() const;

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



    virtual Component* append(Component* component);
	Component* append(const vector<Component*>& components);



public: /** Identifiers Interface */

	const string& id() const;


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

	map <string, std::any>& userData();
    Component* addUserData(const string& key, const std::any& data);
    std::any userData(const string& key);


public: /** Class Interface */

	bool hasClass(const string& className) const;
	Component* removeClass(const string& className);
	Component* addClass(const string& className);
	Component* toggleClass(const string& className);
    const string& classes();
    Component* classes(const string& newClasses);


public: /** Text Interface */
	virtual Component* setText(const string& text);


public: /** Style Interface */
    Component* include(const string& path);


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


};




}