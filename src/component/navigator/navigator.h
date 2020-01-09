#pragma once

#include "../component.h"

namespace Kit
{
    class Window;

    class Navigator : public Component
    {
    public:
        explicit Navigator(Window* parent);

    public:
        void update();

        void setupSize(const Rect& new_size);

    };

}