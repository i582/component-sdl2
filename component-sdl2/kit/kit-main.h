#pragma once

#include "window/window.h"

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

	int count_deleted_windows;

	vector <Window*> windows;

public:
	Window* addWindow(Window* window);

	friend Window;

private:
	void init();
	int setup();
	void render();
	void onEvent();
	void close();

public: /** Interface */
	int run();


public: /** Windows Interface */
	Window* at(size_t index);
	Window* operator[](size_t index);

	vector <Window*>* getWindows();
};

}	
