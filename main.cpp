#include <component-sdl2>
#include <simple-window.h>
#include <select_window.h>

#include "ctime"
#include <fstream>


using namespace Kit;

int main(int, char**)
{
    $.addWindow(new simple_window("Test Window", {-1, -1, 390, 460}, true));

    return $.run();





//
//
//    auto begin = std::chrono::steady_clock::now();
//
//    CSS::css test_css("test.css");
//
//    auto end = std::chrono::steady_clock::now();
//
//
//    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
//    std::cout << "The time: " << elapsed_ms.count() << " ms\n";
//    //std::cout << "Code: " << code << "\n";
//
//    return 0;
}