#include <component-sdl2>
#include <simple-window.h>

using namespace Kit;

int main(int, char**)
{
    $.addWindow(new SimpleWindow("Test Window", {-1, -1, 390, 460}, false));

    return $.run();
}