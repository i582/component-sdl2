#include "simple-window.h"
#include "kit-main.h"


SimpleWindow::SimpleWindow(const string& title, SimpleRect size)
    : Window(title, size)
{
    setup();
}

void SimpleWindow::setup()
{
    include("../src/std_windows/simple_window/css/style.css");
    setDraggableArea({ 0, 0, _size.w - 135, 25 });


    $$->append(new Component("#window-header", { 0, 0, _size.w - 135, 30 }, ".window-header"))
        ->setText(this->title);

    add("#system-exit", ".system-button .system-exit");
    add("#system-expand", ".system-button .system-expand");
    add("#system-collapse", ".system-button .system-collapse");


    Window::getElementById("#system-collapse")->addEventListener("click", [](Component* sender, Event* e)
    {
        sender->window()->collapse();
    });

    add(Button::create("#button-1", "Ok", ".button-ok .button-blue"));
    add(Button::create("#button-cancel", "Cancel", ".button-cancel"));

    add(Checkbox::create("#checkbox", "select this", ".first-checkbox"));



    add(new Table("#table", ".table-test"));

    auto table = (Table*)Window::getElementById("#table");

    table->addCollumn("#");
    table->addCollumn("ФИО");
    table->addCollumn("Русский язык");
    table->addCollumn("Математика");

    table->addRow({ "1", "Молодой человек", "85", "90" });
    table->addRow({ "2", "Старый человек", "75", "100" });
    table->addRow({ "3", "Старый человек", "75", "100" });



    add("#test-text", ".test-text").useExtendedText();
    auto test = Window::getElementById("#test-text");
    test->setText("C:/");

    add(Button::create("#button-browse", "Browse", ".button-browse"));

}
