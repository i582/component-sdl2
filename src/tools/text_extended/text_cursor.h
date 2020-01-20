#pragma once

namespace Kit
{

    struct cursor_position
    {
        int x, y;

        cursor_position() : x(0), y(0){}
        cursor_position(int x, int y) : x(x), y(y){}

        bool operator==(const cursor_position& obj);
    };


}