#include "simple-window.h"
#include "kit-main.h"


SimpleWindow::SimpleWindow(const string& title, SimpleRect size, bool noBorder)
        : Window(title, size, noBorder)
{
    setup();
}

void SimpleWindow::setup()
{
    style("../src/std_windows/simple_window/css/style.css");
    setDraggableArea({0, 0, _size.w - 135, 25});


    $$->append(new Component("#window-header", {0, 0, _size.w - 135, 30}, ".window-header"))
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



    auto table = (Table*) add(new Table("#table", ".table-test"));

    // table->style({{"f: background-color", "#ffffff"}});


    table->addCollumn("#");
    table->addCollumn("ФИО");
    table->addCollumn("Русский язык");
    table->addCollumn("Математика");

    table->addRow({"1", "Молодой человек", "85", "90"});
    table->addRow({"2", "Старый человек", "75", "100"});
    table->addRow({"3", "Старый человек", "75", "100"});


    add("#test-text", ".test-text")->useExtendedText();
    auto test = Window::getElementById("#test-text");
    test->setText("C:/");

    add(Button::create("#button-browse", "Browse", ".button-browse"));

}
