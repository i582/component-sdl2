#pragma once
#include <vector>

#include "text_line.h"
#include "simple-rect.h"
#include "simple-point.h"
#include "utils/utils.h"



namespace Kit
{

class Component;

using std::vector;

class Text2
{
private:
	vector<TextLine*> lines;
	string text;

	SimpleRect size;

	SDL_Renderer* renderer;
	SDL_Texture* texture;
	font font;
	size_t fontSize;
	TTF_Font* fontTTF;
	Color colorFont;

	SDL_Texture* parentTarget;

	double lineHeight;

	CursorPosition cursorPos;

	bool isSelected;
	CursorPosition startCursorSelect;
	CursorPosition endCursorSelect;

	bool mousePush;

public:
	Text2(SDL_Renderer* renderer, const string& text, SimpleRect size, const class font& new_font, size_t fontSize, const Color& colorFont);
	Text2(SDL_Renderer* renderer, SDL_Texture* parentTarget, const string& text, SimpleRect size, const class font& new_font, size_t fontSize, const Color& colorFont);
	~Text2();

public:
	friend TextLine;
	friend Component;

private:
	void splitByLines();
	void setup();

	void splitLinesByToken();

	void renderCursor();

	void handleSelect();
	void deleteSelect();

	CursorPosition whereIsCursor(SimplePoint p);

	string copySelect();

public:
	void render();
	
	void setText(const string& text);

	void mouseMotion(SDL_Event* e);
	void mouseButtonUp(SDL_Event* e);
	void mouseButtonDown(SDL_Event* e);
	void mouseButtonDoubleDown(SDL_Event* e);
	void keyDown(SDL_Event* e);
	void textInput(SDL_Event* e);

};



}