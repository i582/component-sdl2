#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "iostream"
#include "string"

#include "../rect/simple-rect/rect.h"
#include "../point/simple-point/point.h"

namespace Lib
{

using std::string;
using std::to_string;
using std::cout;
using std::endl;

class Component;

class Image
{
private:
	string path;
	SimpleRect textureSize;
	SimpleRect generalSize;
	SimpleRect containerSize;


	SDL_Renderer* renderer;
	SDL_Texture* texture;
	SDL_Texture* parentTexture;

	Component* parent;

	bool needReRender;

public:
	Image(Component* parent);

public:
	void setPath(string path);
	void setRenderer(SDL_Renderer* renderer);

	void setImageSize(SimpleRect generalSize);
	void setImageShift(SimplePoint p);
	void setImageWidth(string size);

private:
	void createTexture();

public:
	void render();

};

}