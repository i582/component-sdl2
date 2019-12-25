#pragma once

#include "../rect/simple-rect/simple-rect.h"
#include "../rect/extended-rect/extended-rect.h"
#include "../css/color/color.h"
#include "../font/font.h"
#include "iostream"
#include "string"
#include "map"

namespace Kit
{

using std::map;
using std::string;
using std::to_string;
using std::cout;
using std::endl;

using CSS::Color;

class Component;

enum class TextAlign
{
	LEFT,
	CENTER,
	RIGHT
};

enum class TextBlockVerticalAlign
{
	TOP,
	CENTER,
	BOTTOM
};

class Text
{
private:
	string text;
	vector<string>* words;
	vector<string> lines;

	Rect size;
	Component* parent;
	SDL_Renderer* renderer;
	SDL_Texture* texture;

	font _font;
	size_t fontSize;
	TTF_Font* ttf_font;
	Color color;

	bool needReRender;


	double lineHeight;
	TextAlign textAlign;
	TextBlockVerticalAlign blockVerticalAlign;
	int blockMarginTop;
	int blockMarginBottom;
	int blockMarginLeft;
	int blockMarginRight;
	int tw;
	int th;
	SimpleRect textRect;
	int textBlockHeight;
	bool splitted;
	int x;
	int y;

public:
	Text(Component* parent, string text, Rect size, font font, size_t fontSize, Color color);
	~Text();

private:
	void init();

	/**
	 * @brief Split text by lines
	 */
	void splitByLines();


	void renderLines();

public:
	void render();

	void setText(const string& text);
	void setSize(const Rect& size);
	void setFont(const font& font);
	void setFontSize(const size_t& fontSize);
	void setColor(const Color& color);
	void setLineHeight(const double& lineHeight);
	void setTextAlign(const string& align);
	void setTextBlockVerticalAlign(const string& align);
	void setTextBlockMargin(const string& side, int value);

	void setRenderer(SDL_Renderer* renderer);


	bool onHover(Point& p);



public:
	SDL_Renderer* getRenderer();
	SDL_Texture* getTexture();

	int getFontSize();
	font& getFont();
	Color getColor();
};

}