#pragma once

#include "../window/window.h"
#include "../tools/rect/extended-rect/rect.h"
#include "../tools/size/simple-size/size.h"
#include "../tools/image/image.h"

#include "component-header.h"


namespace Lib
{

class Component
{
protected:
	/** Sizes */
	Rect _innerSize;
	Rect _outerSize;


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


	/** Events listener */
	map <string, eventCallback> _eventListeners;
	static void _emptyCallback(Component* sender, Event* e) {};


	/** User data */
	map <string, void*> _userData;


	/** State */
	bool _isHovered;
	bool _isActive;


	/** SDL */
	SDL_Renderer* _renderer;
	SDL_Texture* _innerTexture;
	SDL_Texture* _outerTexture;


	/** Styles */ // TO DO
	//CSS::css_block _cssBlock;


	/** Other for Events */
	bool _isEnterInComponent;


	/** Scroll part */ // TO DO
	// Scroll* _scroll;
	// bool _scrollable;


	/** Text part */ // TO DO
	// Font* _font;
	// Text* _text;


	/** Image */
	Image* _image;


public:
	Component(string id, Rect size, string classes);
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
	 * @return The first parent for the current one that needs scrolling
	 */
	Component* getFirstScrollableParent();



protected: /** Setup Functions */

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




public: /** Interface */

	/**
	 * @brief Function to configure the container and all its childs
	 */
	void setupContainer();



public: /** Size Interface */
	
	int width() const;
	int height() const;
	int top() const;
	int left() const;
	const Rect& size() const;
	const Rect& outerSize() const;
	const Rect& innerSize() const;


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


public: /** Hover Interface */

	/**
	 * @brief Checks if a given point is internal to an external size
	 * @return true|false
	 */
	bool onHover(Point point);

	/**
	 * @brief It passes through all the children and finds in which component
	 * the transmitted point is located. Moreover, if the point is in the component,
	 * and at the same time in this component it is in the child, then the child
	 * is returned. That is, the function goes as deep as possible until there
	 * is an unambiguous component in which the point is now
	 */
	Component* const onContainerHover(Point point);


public: /** Identifiers Interface */
	string& id();



public: /** Event listeners Interface */

	/**
	 * @brief Associates the passed callback function with the event with the passed 
	 * identifier. When an event occurs with the transmitted identifier, 
	 * the callback function will be called
	 *
	 * @param action – Event id
	 * @param eventCallback – Functor with form is "function <void (Component * sender, Event * e)>"
	 */
	void addEventListener(string action, eventCallback callback_function);


	/**
	 * @brief Deletes the callback function for the event with the passed identifier
	 *
	 * @param action – Event id
	 */
	void removeEventListener(string action);


	/**
	 * @brief Immediately calls the callback function associated with the event
	 * with the passed identifier
	 *
	 * @param action – Event id
	 * @param e – Event data
	 */
	void callEventListener(string action, Event* e);


public: /** User Data Interface */

	map <string, void*>& userData();
	void addUserData(string key, void* data);
	void* userData(string key);


public: /** State Interface */

	bool isHovered();
	bool isActive();


public: /** Class Interface */

	bool hasClass(string className) const;
	Component* removeClass(string className);
	Component* addClass(string className);
	Component* toggleClass(string className);

};




}