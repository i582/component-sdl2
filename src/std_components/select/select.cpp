#include "select.h"
#include "select_window.h"
#include "kit-main.h"


Kit::Select::Select(const std::string& id_, const std::string& classes_)
        : Component(id_, ".select " + classes_)
{
    Select::setup();

    this->_selectWindow = nullptr;
    this->_windowSize = { 0, 0, 0, 0 };
}

Select::~Select()
{
    delete _selectWindow;
}

Kit::Component* Kit::Select::create(const std::string& id_, const std::string& classes_)
{
    return new Select(id_, classes_);
}

void Kit::Select::setup()
{
    style("../styles/std_components/select/style.css");

    //_selectWindow = new select_window("Select Body", {0, 0, 100, 100}, true);

    append(Component::create("#select-body", ".select-body"))->hide();


    addEventListener("click", [this](Component* sender, Event* e_)
    {
        if (_selectWindow == nullptr)
            return;

        if (_selectWindow->isShow())
        {
            _selectWindow->hide();
        }
        else
        {
            _selectWindow->show();
        }
    });

}

Select* Select::addItem(const string& item_)
{
    _items.push_back(item_);

    handleItems();

    return this;
}

void Select::handleItems()
{
    auto select_body = _childrens[0];

    select_body->append(
         SelectItem::create("#select-item-" + to_string(_items.size()), _items.back(),
                 ".select-item-" + to_string(_items.size()))
    );


    //select_body->setupComponents();




    _windowSize.w = _outerSize.w();
    _windowSize.h = select_body->height();



    const SimpleRect position = {
        parentWindow()->left() + left(),
        parentWindow()->top() + top() + parentWindow()->topBorderSize() + _outerSize.h() + 1,
        0, 0
    };

    _windowSize.x = position.x;
    _windowSize.y = position.y;

    /*delete _selectWindow;
    _selectWindow = new select_window("Select Body", _windowSize, true);
    _selectWindow->render();*/
    //_selectWindow->hide();
    //_windowSize;
}

