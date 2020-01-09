#pragma once

#include "../component.h"

namespace Kit
{
    class window;

    class Navigator : public Component
    {
    public:
        explicit Navigator(window* parent);

    public:
        void update();

        void setupSize(const Rect& new_size);

    };

}