#include "text_cursor.h"

bool Kit::cursor_position::operator==(const Kit::cursor_position& obj)
{
    return this->x == obj.x && this->y == obj.y;
}
