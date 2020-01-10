#include <component-sdl2>
#include <simple-window.h>
#include <select_window.h>

using namespace Kit;

int main(int, char**)
{
    $.addWindow(new simple_window("Test Window", {-1, -1, 390, 460}, true));

    return $.run();
}