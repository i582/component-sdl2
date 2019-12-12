#pragma once

#include "SDL.h"
#include "iostream"
#include "string"

#include "../tools/rect/simple-rect/rect.h"
#include "../event/event.h"

namespace Lib
{
	using std::cout;
	using std::endl;
	using std::string;

class Window
{
private:
	SimpleRect size;
	string title;


	SDL_Window* window;
	SDL_Renderer* renderer;

	bool is_display;


public: /** constructor & destructor*/
	Window(string title, SimpleRect size);
	~Window();

private:
	void init();
	void preSetup();

	virtual void setup() = 0;



private: /** Events */
	virtual void mouseButtonDown(SDL_Event* e) = 0;
	virtual void mouseButtonUp(SDL_Event* e) = 0;
	virtual void mouseMotion(SDL_Event* e) = 0;
	virtual void mouseWheel(SDL_Event* e) = 0;
	virtual void keyDown(SDL_Event* e) = 0;
	virtual void keyUp(SDL_Event* e) = 0;



public: /** Interface */
	virtual void render() = 0;
	virtual void onEvent(Event* e) = 0;


	void show();
	void hide();
	bool isShow();

	void close();
};

}