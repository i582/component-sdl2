#pragma once

#include <iostream>
#include <map>

#include "SDL.h"
#include "SDL_image.h"

#include "../rect/simple-rect/simple-rect.h"
#include "../rect/extended-rect/extended-rect.h"
#include "../point/simple-point/simple-point.h"


namespace Kit
{
    using std::string;
    using std::map;

    class Component;


    class image
    {
    private: // SDL Part
        SDL_Renderer* _renderer;
        SDL_Texture* _texture;
        SDL_Rect _textureSize;

    private: // Image Part
        string _path;
        Rect _size;

        bool _needUpdate;

    private: // Parent Part
        Component* _parent;

    private: // Cache
        static map<string, SDL_Texture*> _cache;

    private:
        void setup();
        void load();


    public:
        explicit image(Component* parent_);

    public: // Main Interface

        image* path(const string& path_);
        image* position(const Point& position_);
        image* size(const Size& size_, bool saveProportion_ = true);

        void render() const;

        static void delete_cache();
    };

}