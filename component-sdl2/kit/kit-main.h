#pragma once

#include "window/window.h"

namespace Lib
{
	using std::cout; 
	using std::endl;


class Kit
{
private:
	static Kit* instance;
	Kit();

public:
	static Kit* init();
	~Kit();


private:
	bool is_running;
	Event e;

	vector <Window*> windows;

public:
	Window* addWindow(Window* window);


private:
	void setup();
	void render();
	void onEvent();
	void close();

public:
	void run();

};

}	
