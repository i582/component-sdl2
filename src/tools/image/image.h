#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "iostream"
#include "string"

#include "../rect/simple-rect/simple-rect.h"
#include "../point/simple-point/simple-point.h"

namespace Kit
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
        void setPath(const string& path);

        void setRenderer(SDL_Renderer* renderer);

        void setImageSize(const SimpleRect& generalSize);

        void setImageShift(const SimplePoint& p);

        void setImageWidth(const string& size);

    private:
        void createTexture();

    public:
        void render();

    };

}