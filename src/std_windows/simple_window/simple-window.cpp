#include "simple-window.h"
#include "kit-main.h"

#include "select_window.h"

simple_window::simple_window(const string& title_, SimpleRect size_, bool noBorder_)
        : window(title_, size_, noBorder_)
{
    simple_window::setup();
}

void simple_window::setup()
{
    style("../styles/std_windows/simple_window/style.css");
    setDraggableArea({0, 0, _size.w - 135, 25});

/*


    _navigator->append(new Component("#window-header", {0, 0, _size.w - 135, 30}, ".window-header"))
            ->setText(_title);

    add("#system-exit", ".system-button .system-exit");
    add("#system-expand", ".system-button .system-expand");
    add("#system-collapse", ".system-button .system-collapse");


    window::getElementById("#system-collapse")->addEventListener("click", [](Component* sender, Event* e_)
    {
        sender->parentWindow()->collapse();
    });

    window::getElementById("#system-exit")->addEventListener("click", [](Component* sender, Event* e_)
    {
        sender->parentWindow()->close();
    });

    add(Button::create("#button-1", "Ok", ".button-ok .button-blue"));
    add(Button::create("#button-cancel", "Cancel", ".button-cancel"));

    add(Checkbox::create("#checkbox", "select this", ".first-checkbox"));

*/

    add("#header", ".header")->setText(_title);

    add("#input", ".input")->useExtendedText();
    auto test = window::getElementById("#input");
    test->setText("\n #;Name;Phone;Street\n 1;Sasha;+123456789;number-1\n 2;Egor;+123456789;number-2\n 3;Evgenia;+123456789;number-3");


    add("#input-one-line", ".input-one-line")->useExtendedText()
    ->setText("Search")->oneLineText();

    add(Checkbox::create("#checkbox", "Save changes", ".first-checkbox"));

    add(Button::create("#button-1", "Ok", ".button-ok .button-blue"));
    add(Button::create("#button-cancel", "Cancel", ".button-cancel"));


    add("#button-next", ".button-next");
    add("#button-prev", ".button-prev");


    add(LineSelect::create("#line-select", {"item 1", "item 2", "item 3", "item 4"}, ".bar"));

    /*auto table = (Table*) add(new Table("#table", ".table-test"));

    table->addColumn("#");
    table->addColumn("Name");
    table->addColumn("Phone");
    table->addColumn("Street");

    table->addRow({"1", "Sasha", "+123456789", "number-1"});
    table->addRow({"2", "Egor", "+123456789", "number-2"});
    table->addRow({"3", "Evgenia", "+123456789", "number-3"});
*/

    //add("#test-scroll", ".scrolls")->append(Component::create("#inner-scroll", ".inner-scroll"));

    /*add("#test-text", ".test-text")->useExtendedText();
    auto test = window::getElementById("#test-text");
    test->setText("This is just simple text for check!");*/



//    add(Button::create("#button-browse", "Browse", ".button-browse"));

//    auto select = (Select*)add(Select::create("#test-select", ".select-test"));
//
//    select->addItem("Item");



/*
    auto select = (Select*)add(Select::create("#test-select", ".select-test"));

    select->addItem("Item");*/

/*
    window::getElementById("#button-browse")->addEventListener("click", [](Component* sender, Event* e_)
    {
        const SimpleRect position = {
            sender->parentWindow()->left() + sender->left() - 20,
            sender->parentWindow()->top() + sender->top() - 100 /*+ sender->parentWindow()->topBorderSize()*//*,
           0, 0
        };



        sender->parentWindow()->parent()->addWindow(
             new select_window("Select", {position.x, position.y, 100, 100}, true)
        );
    });
*/
    //.addWindow(new select_window("Select", {100, 100, 100, 100}, false));
}
