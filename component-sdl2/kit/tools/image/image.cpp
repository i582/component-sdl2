#include "image.h"
#include "../../component/component.h"

using namespace Lib;

Image::Image(Component* parent)
{
	if (parent == nullptr)
		return;

	IMG_Init(IMG_INIT_PNG);

	this->parent = parent;
	this->renderer = parent->renderer();
	this->parentTexture = parent->innerTexture();

	this->containerSize = this->generalSize = { 0, 0 , parent->width(), parent->height() };

	this->needReRender = true;
}

void Image::setPath(string path)
{
	this->path = path;
	this->needReRender = true;
}

void Image::setRenderer(SDL_Renderer* renderer)
{
	this->renderer = renderer;
}

void Image::setImageSize(SimpleRect generalSize)
{
	this->generalSize = generalSize;
}

void Image::setImageShift(SimplePoint p)
{
	this->generalSize.x = p.x;
	this->generalSize.y = p.y;
}

void Image::setImageWidth(string size)
{
	if (size == "")
		return;


	int newWidth = Point::parseStringToNumber(size, parent->width());

	double imageRatio = this->generalSize.w / (double)this->generalSize.h;

	this->generalSize.w = newWidth;
	this->generalSize.h = newWidth / imageRatio;

}

void Image::createTexture()
{
	if (this->texture != nullptr)
	{
		SDL_DestroyTexture(this->texture);
	}

	if (this->path.empty())
		return;

	this->texture = IMG_LoadTexture(renderer, this->path.c_str());

	if (this->texture == nullptr)
	{
		cout << "Error: " << IMG_GetError() << endl;
		return;
	}

	SDL_QueryTexture(this->texture, nullptr, nullptr, &textureSize.w, &textureSize.h);

	needReRender = false;
}

void Image::render()
{
	if (needReRender)
		createTexture();

	SDL_SetRenderTarget(renderer, this->parentTexture);

	SimpleRect srcRect = { 0, 0, generalSize.w, generalSize.h };

	SDL_RenderCopy(renderer, this->texture, NULL, &generalSize);
}
