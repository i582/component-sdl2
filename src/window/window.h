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
	Component* getElementById(const string& id) const;
	Components getElementsByClassName(const string& className) const;

	Component& add(const string& id, const string& classes, const vector<Component*>& childrens = {});
    Component& add(const string& classes = "", const vector<Component*>& childrens = {});
	Component& add(Component* component);

	Component* create(const string& id, const string& classes, const vector<Component*>& childrens = {});
    Component* create(const string& classes = "", const vector<Component*>& childrens = {});
	Component* create(Component* component);

public: /** Component Style Interface */

	CSS::css_block* addStyle(const string& className, CSS::css_block style);


public:
    Component* focusComponent;


protected:
	SimpleRect _size;
	string title;
	size_t _id;

	SDL_Window* window;
	SDL_Renderer* renderer;

	bool is_display;

	Navigator* navigator;
	Navigator* $$;

	CSS::css main_css;
	bool wasSetupStyle;
	bool wasSetupComponents;


	KitApplication* parent;

public: /** constructor & destructor*/
	Window(const string& title, SimpleRect size);
	~Window();

public:
	friend Component;
	friend KitApplication;

protected:
	void init();
	void preSetup();

	virtual void setup() {};



protected: /** Events */
	void mouseButtonDown(Event* e);
	void mouseButtonUp(Event* e);
	void mouseMotion(Event* e);
	void mouseWheel(Event* e);
	void keyDown(Event* e);
	void keyUp(Event* e);
    void textInput(Event* e);

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
	void include(const string& path);

};

}