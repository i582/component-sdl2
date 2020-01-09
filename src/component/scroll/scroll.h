#pragma once

#include "simple-rect.h"
#include "texture.h"

namespace Kit
{
    class Component;

    class scroll
    {
    protected: // SDL Part
        SDL_Renderer* _renderer;
        SDL_Texture* _texture;

    protected: // Sizes
        SimpleRect _body;
        SimpleRect _slider;

    protected: // Parent
        Component* _parent;

    protected: // Main Variable

        int _maxValue;
        int _nowValue;

        double _aspectRatio;
        double _nowPosition;

    protected:
        void init();


    public:
        explicit scroll(Component* parent_);
        virtual ~scroll();

    public:
        bool onHover(const Point& point_);
        bool onSliderHover(const Point& point_);

    public:
        virtual void shift(int delta_);

    public:
        virtual void setup(const SimpleRect& body_, const SimpleRect& slider_, int maxValue_, int nowValue_,
                double aspectRatio_, double nowPosition_);

    public:
        void render() const;

    };
}