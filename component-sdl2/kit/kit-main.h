#pragma once

#include "iostream"
#include "SDL.h"
#include "event/event.h"


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

private:
	void setup();
	void render();
	void onEvent();
	void close();

public:
	void run();

};

}	
