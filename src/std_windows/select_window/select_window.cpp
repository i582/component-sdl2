#include "select_window.h"
#include "kit-main.h"


select_window::select_window(const string& title_, const SimpleRect& size_, bool noBorder_)
        : window(title_, size_, noBorder_)
{
    select_window::setup();
}

void select_window::setup()
{
    style("../styles/std_windows/select_window/style.css");

    addEventListener("focus_lost", [](window* sender, Event* e)
    {
        cout << "window was lost focus" << endl;
        sender->hide();
    });
}
