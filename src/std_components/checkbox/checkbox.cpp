#include "checkbox.h"

#include <utility>

Checkbox::Checkbox(const string& id, const string& text, const string& classes,
                   function<void(Component* sender, Event* e)> callback)
        : Component(id, ".checkbox " + classes)
{
    this->callback = std::move(callback);
    this->text = text;
    this->state = CheckboxState::UNCHECKED;
    setup();
}


void Checkbox::setup()
{
    style("../src/std_components/checkbox/css/style.css");

    this->addEventListener("change", [&](Component* sender, Event* e)
    {
        callback(sender, e);
    });

    this->addEventListener("onmouseup", [&](Component* sender, Event* e)
    {
        if (state == CheckboxState::CHECKED)
        {
            sender->removeClass(".checkbox-checked");
            state = CheckboxState::UNCHECKED;
        }
        else
        {
            sender->addClass(".checkbox-checked");
            state = CheckboxState::CHECKED;
        }

        callEventListener("change", e);
    });

    setText(text);
}

bool Checkbox::isChecked() const
{
    return state == CheckboxState::CHECKED;
}

Checkbox* Checkbox::create(const string& id, const string& text, const string& classes,
                           function<void(Component*, Event*)> callback)
{
    return new Checkbox(id, text, classes, std::move(callback));
}
