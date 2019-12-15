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

	int count_deleted_windows;

	vector <Window*> windows;

public:
	Window* addWindow(Window* window);

	friend Window;

private:
	void setup();
	void render();
	void onEvent();
	void close();

public: /** Interface */
	void run();

	Window* at(size_t index);
	Window* operator[](size_t index);

	vector <Window*>* getWindows();
};

}	
