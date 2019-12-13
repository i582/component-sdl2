#pragma once

#include "SDL.h"
#include "iostream"
#include "string"

#include "../tools/rect/simple-rect/simple-rect.h"
#include "../event/event.h"

#include "../component/navigator/navigator.h"
#include "../component/components/components.h"


namespace Lib
{
	using std::cout;
	using std::endl;
	using std::string;

	class Kit;

class Window
{

protected:
	map <string, Component*> allComponents;
	map <string, CSS::css_block> allComponentsStyles;

protected:
	/**
	 * @brief A function that processes all elements and adds styles
	 * associated with classes for each component
	 */
	void handleStyles();

public: /** Component Interface */

	Component* addElement(Component* component);
	Component* getElementById(string id) const;
	Components getElementsByClassName(string className) const;



public: /** Component Style Interface */

	CSS::css_block* addStyle(string className, CSS::css_block style);
	


protected:
	SimpleRect _size;
	string title;


	SDL_Window* window;
	SDL_Renderer* renderer;

	bool is_display;

	Navigator* navigator;
	Navigator* $$;

	CSS::css main_css;
	bool wasSetupStyle;
	bool wasSetupComponents;


	Kit* parent;

public: /** constructor & destructor*/
	Window(string title, SimpleRect size);
	~Window();

public:
	friend Component;
	friend Kit;

protected:
	void init();
	void preSetup();

	virtual void setup() = 0;



protected: /** Events */
	void mouseButtonDown(SDL_Event* e);
	void mouseButtonUp(SDL_Event* e);
	void mouseMotion(SDL_Event* e);
	void mouseWheel(SDL_Event* e);
	void keyDown(SDL_Event* e);
	void keyUp(SDL_Event* e);


public: /** Interface */
	void render();
	void onEvent(Event* e);


	void show();
	void hide();
	bool isShow();

	void collapse();

	void close();




public: /** Size Interface */

	int width() const;
	int height() const;
	int top() const;
	int left() const;
	SimpleRect size() const;



public: /** SDL Interface */

	SDL_Renderer* getRenderer() const;
	SDL_Window* getWindow() const;


public: /** Other Interface */

	void setDraggableArea(SimpleRect area);

public: /** CSS Interface */

	/**
	 *  @brief Function for include css style file
	 */
	void include(string path);

};

}