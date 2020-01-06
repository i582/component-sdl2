#include <component-sdl2>

using namespace Kit;

int main(int, char**)
{
    auto window = $.addWindow(new Window("Test Window", { -1, -1, 300, 300 }, false));

    // Добавляем стили в окно
    window->style("../src/std_windows/simple_window/css/style.css");

    window->add("#button", ".button");

    return $.run();
}