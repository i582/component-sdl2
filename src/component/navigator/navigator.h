#pragma once

#include "../component.h"

namespace Kit
{
    class window;

    class Select;

    class Navigator : public Component
    {
    public:
        explicit Navigator(window* parent);

    public:
        friend Select;

    public:
        void update();

        void setupSize(const Rect& new_size);

    };

}