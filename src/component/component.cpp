#include "component.h"
#include "../window/window.h"

Kit::Component* Kit::Component::_hoverComponent = nullptr;

Kit::Component::Component(const string &id, const Rect &size, const string &classes) : Component(id, size, classes, {}) {}

Kit::Component::Component(const string &id, const Rect &size, const string &classes,
                          const vector<Component*> &childrens)
{
    /** Sizes */
    this->_innerSize = size;
    this->_outerSize = size;
    this->_isConstructSize = true;

    this->_autoWidth = false;
    this->_autoHeight = false;

    /** Identifiers */
    this->_id = id;
    this->_classes = classes;


    /** Parents */
    this->_parent = nullptr;
    this->_window = nullptr;


    /** Childrens */
    // this->_childrens = childrens;
    for (auto &children : childrens)
    {
        append(children);
    }

    this->_childrensSize;


    /** Display */
    this->_isDisplay = true;



    /** Events listener */
    this->_eventListeners = {};
    this->setupEventListeners();


    /** User data */
    this->_userData;


    /** State */
    this->_isHovered = false;
    this->_isActive = false;
    this->_isFocused = false;


    /** SDL */
    this->_renderer = nullptr;
    this->_innerTexture = nullptr;
    this->_outerTexture = nullptr;


    /** Styles */
    this->_cssBlock = CSS::css_block(id, true);


    /** Other for Events */
    this->_isEnterInComponent = false;


    /** Font */
    this->_fontNormal = font();
    this->_fontHover = font();
    this->_fontActive = font();


    /** Text */
    this->_text = nullptr;
    this->_text_temp = "";


    /** Vertical Scroll */
    this->_verticalScroll = new VerticalScroll(_renderer, {0, 0, 15, 0}, 0, 1);
    this->_verticalScrollable = false;
    this->_isVerticalScrollActive = false;


    /** Horizontal Scroll */
    this->_horizontalScroll = new HorizontalScroll(_renderer, {0, 0, 0, 15}, 0, 1);
    this->_horizontalScrollable = false;
    this->_isHorizontalScrollActive = false;

    this->_needRenderScroll = true;


    /** Image */
    this->_image = nullptr;


    /** CSS component */
    this->_css_component = nullptr;


    /** Animation */
    this->_animation = new animation<int>(100, 900, 2000, this->_outerSize.start.px(), []()
    {
    });


    /** Extended text */
    //this->_extended_text = nullptr;
}

Kit::Component::Component(const string &id, const string &classes, const vector<Component*> &childrens)
{
    /** Sizes */
    this->_innerSize;
    this->_outerSize;
    this->_isConstructSize = false;

    this->_autoWidth = false;
    this->_autoHeight = false;


    /** Identifiers */
    this->_id = id;
    this->_classes = classes;


    /** Parents */
    this->_parent = nullptr;
    this->_window = nullptr;


    /** Childrens */
    // this->_childrens = childrens;
    for (auto &children : childrens)
    {
        append(children);
    }

    this->_childrensSize;


    /** Display */
    this->_isDisplay = true;



    /** Events listener */
    this->_eventListeners = {};
    this->setupEventListeners();


    /** User data */
    this->_userData;


    /** State */
    this->_isHovered = false;
    this->_isActive = false;
    this->_isFocused = false;


    /** SDL */
    this->_renderer = nullptr;
    this->_innerTexture = nullptr;
    this->_outerTexture = nullptr;


    /** Styles */
    this->_cssBlock = CSS::css_block(id);


    /** Other for Events */
    this->_isEnterInComponent = false;


    /** Font */
    this->_fontNormal = font();
    this->_fontHover = font();
    this->_fontActive = font();


    /** Text */
    this->_text = nullptr;
    this->_text_temp = "";


    /** Vertical Scroll */
    this->_verticalScroll = new VerticalScroll(_renderer, {0, 0, 15, 0}, 0, 1);
    this->_verticalScrollable = false;
    this->_isVerticalScrollActive = false;


    /** Horizontal Scroll */
    this->_horizontalScroll = new HorizontalScroll(_renderer, {0, 0, 0, 15}, 0, 1);
    this->_horizontalScrollable = false;
    this->_isHorizontalScrollActive = false;

    this->_needRenderScroll = true;


    /** Image */
    this->_image = nullptr;


    /** CSS component */
    this->_css_component = nullptr;


    /** Animation */
    this->_animation = new animation<int>(100, 900, 2000, this->_outerSize.start.px(), []()
    {
    });


    /** Extended text */
    //this->_extended_text = nullptr;
}

Kit::Component::~Component()
{
    SDL_DestroyTexture(_innerTexture);
    SDL_DestroyTexture(_outerTexture);

    delete _verticalScroll;
    delete _horizontalScroll;

    delete _text;
    delete _image;
    delete _css_component;
}

void Kit::Component::setupEventListeners()
{
    _eventListeners["click"] = Component::_emptyCallback;
    _eventListeners["hover"] = Component::_emptyCallback;
    _eventListeners["mousemotion"] = Component::_emptyCallback;
    _eventListeners["onmousedown"] = Component::_emptyCallback;
    _eventListeners["onmouseup"] = Component::_emptyCallback;
    _eventListeners["onmouseover"] = Component::_emptyCallback;
    _eventListeners["onmouseout"] = Component::_emptyCallback;
}


Kit::Component* Kit::Component::getFirstVerticalScrollableParent()
{
    if (this->_parent != nullptr)
    {
        if (this->_parent->isVerticalScrollable())
        {
            return this->_parent;
        }
        else
        {
            return this->_parent->getFirstVerticalScrollableParent();
        }
    }

    return nullptr;
}

Kit::Component* Kit::Component::getFirstHorizontalScrollableParent()
{
    if (this->_parent != nullptr)
    {
        if (this->_parent->isHorizontalScrollable())
        {
            return this->_parent;
        }
        else
        {
            return this->_parent->getFirstHorizontalScrollableParent();
        }
    }

    return nullptr;
}

CSS::css* Kit::Component::getComponentStyles()
{
    return this->_css_component;
}

void Kit::Component::setupSize()
{
    if (!this->_isConstructSize)
    {
        const auto& width = _cssBlock.normal().get<string>("width");
        const auto& height = _cssBlock.normal().get<string>("height");
        const auto& left = _cssBlock.normal().get<string>("left");
        const auto& top = _cssBlock.normal().get<string>("top");


        _autoWidth = (width == "auto");
        _autoHeight = (height == "auto");


        Rect size = Rect(left, top, width, height);

        this->_innerSize = size;
        this->_outerSize = size;
    }

    for (auto &children : _childrens)
    {
        children->setupSize();
    }
}

void Kit::Component::setupChildrenRenderer()
{
    if (this->_renderer == nullptr && this->_parent != nullptr)
    {
        this->_renderer = _parent->renderer();
    }

    for (auto &children : _childrens)
    {
        children->setupChildrenRenderer();
    }
}

void Kit::Component::computeSize()
{

    if (_parent != nullptr)
    {
        const Rect& parentSize = _parent->size();
        _innerSize.calc(parentSize);
        _outerSize.calc(parentSize);
    }


    /** find the maximum border size */


    auto& normalBlock = _cssBlock.normal();
    auto& hoverBlock = _cssBlock.hover();
    auto& activeBlock = _cssBlock.active();
    const auto topSize = Utils::max_of(
            normalBlock.get<int>("border-top-size"),
            hoverBlock.get<int>("border-top-size"),
            activeBlock.get<int>("border-top-size")
    );
    const auto bottomSize = Utils::max_of(
            normalBlock.get<int>("border-top-size"),
            hoverBlock.get<int>("border-top-size"),
            activeBlock.get<int>("border-top-size")
    );
    const auto leftSize = Utils::max_of(
            normalBlock.get<int>("border-top-size"),
            hoverBlock.get<int>("border-top-size"),
            activeBlock.get<int>("border-top-size")
    );
    const auto rightSize = Utils::max_of(
            normalBlock.get<int>("border-top-size"),
            hoverBlock.get<int>("border-top-size"),
            activeBlock.get<int>("border-top-size")
    );


    _outerSize.size.dw(leftSize + rightSize);
    _outerSize.size.dh(topSize + bottomSize);


    _outerSize.start.dx(-leftSize);
    _outerSize.start.dy(-topSize);


    _innerSize.start.x(leftSize);
    _innerSize.start.y(topSize);



    Texture::destroy(this->_outerTexture);
    this->_outerTexture = Texture::create(_renderer, _outerSize);


    Texture::destroy(this->_innerTexture);
    this->_innerTexture = Texture::create(_renderer, _innerSize);


    for (auto &children : _childrens)
    {
        children->computeSize();
    }
}

void Kit::Component::computeChildrenSize()
{
    for (auto &children : _childrens)
    {
        children->computeChildrenSize();
    }


    SimpleSize childrenSize = {0, 0};

    for (auto &children : _childrens)
    {
        if (!children->display())
            continue;

        int width_now_child = children->width() + children->left();
        int height_now_child = children->height() + children->top();

        if (width_now_child > childrenSize.w)
            childrenSize.w = width_now_child;

        if (height_now_child > childrenSize.h)
            childrenSize.h = height_now_child;
    }

    _childrensSize = {0, 0, childrenSize.w, childrenSize.h};


    Size newTextureSize = _innerSize.size;


    if (!_autoWidth && childrenSize.w > _innerSize.w())
    {
        newTextureSize.w(childrenSize.w);


        /*
         *  TODO: Setup scroll
         */
        this->_horizontalScroll->_bodySize.x(_innerSize.x());
        this->_horizontalScroll->_bodySize.y(_innerSize.y() + _innerSize.h() - this->_horizontalScroll->_bodySize.h());

        this->_horizontalScroll->_bodySize.w(_innerSize.w());

        this->_horizontalScroll->_maxValue = newTextureSize.w() - _innerSize.w();

        this->_horizontalScroll->_relSizes = _innerSize.w() / (double) newTextureSize.w();
        this->_horizontalScroll->_renderer = _renderer;
        this->_horizontalScroll->init();



        //_outerSize.dh(this->_horizontalScroll->_bodySize.h());

        _innerSize.dh(-this->_horizontalScroll->_bodySize.h());

        Texture::destroy(this->_innerTexture);
        this->_innerTexture = Texture::create(_renderer, _innerSize);



        this->_horizontalScroll->_parentTexture = this->_outerTexture;
        this->_horizontalScrollable = true;

    }
    else if (_autoWidth)
    {
        newTextureSize.w(childrenSize.w);

        this->_innerSize.w(childrenSize.w);
        this->_outerSize.w(childrenSize.w);

        Texture::destroy(this->_outerTexture);
        this->_outerTexture = Texture::create(_renderer, _outerSize);
    }


    if (!_autoHeight && childrenSize.h > _innerSize.h())
    {
        newTextureSize.h(childrenSize.h);



        /*
         *  TODO: Setup scroll
         */
        this->_verticalScroll->_bodySize.x(_innerSize.x() + _innerSize.w() -  this->_verticalScroll->_bodySize.w());
        this->_verticalScroll->_bodySize.y(_innerSize.y());

        this->_verticalScroll->_bodySize.h(_innerSize.h());

        this->_verticalScroll->_maxValue = newTextureSize.h() - _innerSize.h();

        this->_verticalScroll->_relSizes = _innerSize.h() / (double) newTextureSize.h();
        this->_verticalScroll->_renderer = _renderer;
        this->_verticalScroll->init();

        //_outerSize.dw(this->_verticalScroll->_bodySize.w());


        _innerSize.dw(-this->_verticalScroll->_bodySize.w());

        Texture::destroy(this->_innerTexture);
        this->_innerTexture = Texture::create(_renderer, _innerSize);


        this->_verticalScroll->_parentTexture = this->_outerTexture;
        this->_verticalScrollable = true;

    }
    else if (_autoHeight)
    {
        newTextureSize.h(childrenSize.h);

        this->_innerSize.h(childrenSize.h);
        this->_outerSize.h(childrenSize.h);

        Texture::destroy(this->_outerTexture);
        this->_outerTexture = Texture::create(_renderer, _outerSize);
    }


    Texture::destroy(this->_innerTexture);
    this->_innerTexture = Texture::create(_renderer, newTextureSize);
}

void Kit::Component::setupBackgroundImage()
{
    this->_image = new Image(this);

    for (auto &children : _childrens)
    {
        children->setupBackgroundImage();
    }
}

void Kit::Component::setupFont()
{
    auto font_family = _cssBlock.normal().get<string>("font-family");
    auto font_style = _cssBlock.normal().get<string>("font-style");
    auto font_weight = _cssBlock.normal().get<int>("font-weight");


    this->_fontNormal = font(font_family, font_style, font_weight);


    font_family = _cssBlock.hover().get<string>("font-family");
    font_style = _cssBlock.hover().get<string>("font-style");
    font_weight = _cssBlock.hover().get<int>("font-weight");

    this->_fontHover = font(font_family, font_style, font_weight);


    font_family = _cssBlock.active().get<string>("font-family");
    font_style = _cssBlock.active().get<string>("font-style");
    font_weight = _cssBlock.active().get<int>("font-weight");

    this->_fontActive = font(font_family, font_style, font_weight);


    font_family = _cssBlock.focus().get<string>("font-family");
    font_style = _cssBlock.focus().get<string>("font-style");
    font_weight = _cssBlock.focus().get<int>("font-weight");

    this->_fontFocus = font(font_family, font_style, font_weight);


    for (auto &children : _childrens)
    {
        children->setupFont();
    }
}

void Kit::Component::setupText()
{
    auto font_size = _cssBlock.normal().get<int>("font-size");

    this->_text = new Text(this, _text_temp, {0, 0, _innerSize.w(), _innerSize.h()}, _fontNormal, font_size,
                           Color(0x00000000));
    this->_text->setRenderer(this->_renderer);

    for (auto &children : _childrens)
    {
        children->setupText();
    }
}

void Kit::Component::setupExtendedText()
{
    auto font_size = _cssBlock.normal().get<int>("font-size");

    //this->_extended_text = new Text2(_renderer, _innerTexture, "Hello", {0, 0, _innerSize.w(), _innerSize.h()},
    //        _fontNormal, font_size,Color(0x00000000));



    for (auto &children : _childrens)
    {
        children->setupExtendedText();
    }
}

void Kit::Component::setupParentWindow()
{
    if (this->_window == nullptr && _parent != nullptr && _parent->_window != nullptr)
    {
        this->_window = _parent->_window;
        this->_window->addElement(this);
    }

    for (auto &children : _childrens)
    {
        children->setupParentWindow();
    }
}

void Kit::Component::setupChildrenPosition()
{
    const auto& display_type = _cssBlock.normal().get<string>("display");

    if (!_childrens.empty())
    {

        if (display_type == "block")
        {
            auto& first_children = _childrens[0];

            first_children->outerLeft(0);
            first_children->outerTop(0);


            for (size_t i = 1; i < _childrens.size(); ++i)
            {
                auto& children = _childrens[i];
                auto& children_prev = _childrens[i - 1];

                const int new_y_position = children_prev->outerSize().h() + children_prev->outerSize().y();

                children->outerLeft(0);
                children->outerTop(new_y_position);
            }
        }

    }


    for (auto &children : _childrens)
    {
        children->setupChildrenPosition();
    }
}

void Kit::Component::adjustMousePoint(Point &p)
{
    if (_parent == nullptr)
        return;

    p = p - _outerSize.start;

    _parent->adjustMousePoint(p);
}

void Kit::Component::render()
{
    if (!_isDisplay || (_outerTexture == nullptr && _innerTexture == nullptr))
        return;

    SDL_SetRenderTarget(_renderer, _outerTexture);
    SDL_SetRenderDrawColor(_renderer, 0xff, 0xff, 0xff, 0x00);
    SDL_RenderClear(_renderer);

    SDL_SetRenderTarget(_renderer, _innerTexture);
    SDL_SetRenderDrawColor(_renderer, 0xff, 0xff, 0xff, 0x00);
    SDL_RenderClear(_renderer);


    CSS::css_block_state* blockState = &_cssBlock.normal();

    font& blockFont = _fontNormal;


    if (_isFocused)
    {
        blockState = &_cssBlock.focus();
        blockFont = _fontFocus;
    }

    if (_isHovered)
    {
        if (_isActive)
        {
            blockState = &_cssBlock.active();
            blockFont = _fontActive;
        }
        else
        {
            blockState = &_cssBlock.hover();
            blockFont = _fontHover;
        }
    }


    SDL_SetRenderTarget(_renderer, _innerTexture);


    const auto& backgroundColor = blockState->get<Color>("background-color");
    const auto& borderColor = blockState->get<Color>("border-color");
    const auto& borderRadius = blockState->get<int>("border-radius");


    //roundedBoxColor(_renderer, 0, 0, _innerSize.w(), _innerSize.h(), borderRadius, backgroundColor.colorReverse());


    const SimpleRect _innerRectSize = { 0, 0, _innerSize.w(), _innerSize.h() };

    Draw::roundedRect(_renderer, _innerRectSize, borderRadius, backgroundColor);


    if (this->_image != nullptr)
    {

        this->_image->setPath(blockState->get<string>("background-image"));


        const int &x_shift = blockState->get<int>("background-position-x");
        const int &y_shift = blockState->get<int>("background-position-y");
        this->_image->setImageShift({x_shift, y_shift});


        const string &newSize = blockState->get<string>("background-size");
        this->_image->setImageWidth(newSize);


        this->_image->render();
    }


    _text->setFont(blockFont);
    _text->setColor(blockState->get<Color>("color"));
    _text->setFontSize(blockState->get<int>("font-size"));
    _text->setLineHeight(blockState->get<double>("line-height"));
    _text->setTextAlign(blockState->get<string>("text-align"));
    _text->setTextBlockVerticalAlign(blockState->get<string>("vertical-align"));

    _text->setTextBlockMargin("top", blockState->get<int>("margin-top"));
    _text->setTextBlockMargin("bottom", blockState->get<int>("margin-bottom"));
    _text->setTextBlockMargin("left", blockState->get<int>("margin-left"));
    _text->setTextBlockMargin("right", blockState->get<int>("margin-right"));

    _text->render();


//    if (this->_extended_text != nullptr)
//        this->_extended_text->render();


    for (auto &children : _childrens)
    {
        children->render();
    }


    SDL_SetRenderTarget(_renderer, _outerTexture);


    const auto &topSize = blockState->get<int>("border-top-size");
    const auto &bottomSize = blockState->get<int>("border-bottom-size");
    const auto &leftSize = blockState->get<int>("border-left-size");
    const auto &rightSize = blockState->get<int>("border-right-size");


   /* roundedBoxColor(_renderer, _innerSize.x() - leftSize, _innerSize.y() - topSize,
                    _innerSize.x() + _innerSize.w() - 1 + rightSize, _innerSize.y() + _innerSize.h() - 1 + bottomSize,
                    borderRadius, borderColor.colorReverse());*/

    const SDL_Rect _innerRectBorderSize =
    {
            _innerSize.x() - leftSize,
            _innerSize.y() - topSize,
            _innerSize.w() + leftSize + rightSize,
            _innerSize.h() + topSize + bottomSize
    };


    Draw::roundedRect(_renderer, _innerRectBorderSize, borderRadius + 2, borderColor);



//    filledEllipseColor(_renderer, 20, 20, 20, 21, 0xffffffff);
    //aacircleColor(_renderer, 20, 20, 3, 0x5E6060fe);
//
//    filledEllipseColor(_renderer, 20, 20, 19, 20, 0xCE4848ff);
//    aacircleColor(_renderer, 20, 20, 20, 0xCE4848ff);



    Rect copy = _innerSize;


    const string& overflow = blockState->get<string>("overflow");


    if (overflow == "hidden")
    {
        copy.y(0);
        copy.x(0);
        _needRenderScroll = false;
        _verticalScrollable = false;
    }
    else
    {
        copy.x(this->_horizontalScroll->_nowValue);
        copy.y(this->_verticalScroll->_nowValue);
    }


    SDL_RenderCopy(_renderer, _innerTexture, &copy.toSdlRect(), &_innerSize.toSdlRect());


    if (_needRenderScroll)
        _verticalScroll->render();

    _horizontalScroll->render();


    SDL_Texture* parentTexture = nullptr;
    if (_parent != nullptr)
    {
        parentTexture = _parent->innerTexture();
    }

    SDL_SetRenderTarget(_renderer, parentTexture);
    SDL_RenderCopy(_renderer, _outerTexture, nullptr, &_outerSize.toSdlRect());
}

void Kit::Component::mouseButtonDown(Event* e)
{
    if (!_isDisplay)
        return;


    Point mouseP(e->motion.x, e->motion.y);

    adjustMousePoint(mouseP);


    if (_verticalScroll->onHoverSlider(mouseP))
    {
        cout << "_verticalScroll slider" << endl;

        cout << "_verticalScroll active TRUE" << endl;
        _isVerticalScrollActive = true;

        return;
    }

    if (_horizontalScroll->onHoverSlider(mouseP))
    {
        cout << "_horizontalScroll slider" << endl;

        cout << "_horizontalScroll active TRUE" << endl;
        _isHorizontalScrollActive = true;

        return;
    }


    _eventListeners["click"](this, e);
    _eventListeners["onmousedown"](this, e);

    _isActive = true;
}

void Kit::Component::mouseButtonUp(Event* e)
{
    if (!_isDisplay)
        return;

    _eventListeners["onmouseup"](this, e);


    _isVerticalScrollActive = false;
    _isHorizontalScrollActive = false;
    _isActive = false;
}

void Kit::Component::mouseMotion(Event* e)
{
    if (!_isDisplay)
        return;

    if (_isVerticalScrollActive)
    {
        _verticalScroll->shift(e->motion.yrel * (_childrensSize.h() / _innerSize.h()));
        return;
    }

    if (_isHorizontalScrollActive)
    {
        _horizontalScroll->shift(e->motion.xrel * (_childrensSize.w() / _innerSize.w()));
        return;
    }

    if (!_isEnterInComponent)
    {
        _eventListeners["onmouseover"](this, e);
        _isEnterInComponent = true;
    }

    if (_hoverComponent != this)
    {
        if (_hoverComponent != nullptr && !_hoverComponent->isChildrenObject(this))
        {
            _hoverComponent->mouseOut(e);
        }

        _hoverComponent = this;
    }


    _isHovered = true;

    _eventListeners["mousemotion"](this, e);
    _eventListeners["hover"](this, e);
}

void Kit::Component::mouseOut(Event* e)
{
    if (!_isDisplay)
        return;

    _eventListeners["onmouseout"](this, e);

    _isHovered = false;
    _isEnterInComponent = false;
}

void Kit::Component::mouseScroll(Event* e, int scrollDirection)
{
    if (!_isDisplay)
        return;

    if (SDL_GetModState() & KMOD_SHIFT)
    {
        if (!_horizontalScrollable)
        {
            Component* firstScrollableParent = getFirstHorizontalScrollableParent();

            if (firstScrollableParent != nullptr)
            {
                firstScrollableParent->mouseScroll(e, scrollDirection);
            }
        }
        else
        {
            if (scrollDirection < 0)
                this->_horizontalScroll->shift(20);
            else
                this->_horizontalScroll->shift(-20);
        }
    }
    else
    {
        if (!_verticalScrollable)
        {
            Component* firstScrollableParent = getFirstVerticalScrollableParent();

            if (firstScrollableParent != nullptr)
            {
                firstScrollableParent->mouseScroll(e, scrollDirection);
            }
        }
        else
        {
            if (scrollDirection < 0)
                this->_verticalScroll->shift(20);
            else
                this->_verticalScroll->shift(-20);
        }
    }

}

void Kit::Component::setupComponents()
{
    setupChildrenRenderer();

    setupSize();
    computeSize();

    setupChildrenPosition();
    computeChildrenSize();


    setupBackgroundImage();
    setupFont();
    setupText();

    setupExtendedText();
}


int Kit::Component::width() const
{
    return _outerSize.w();
}

int Kit::Component::height() const
{
    return _outerSize.h();
}

int Kit::Component::top() const
{
    return _outerSize.y();
}

int Kit::Component::left() const
{
    return _outerSize.x();
}

const Kit::Rect &Kit::Component::size() const
{
    return _innerSize;
}

const Kit::Rect &Kit::Component::outerSize() const
{
    return _outerSize;
}

const Kit::Rect &Kit::Component::innerSize() const
{
    return _innerSize;
}

Kit::Component* Kit::Component::show()
{
    _isDisplay = true;
    return this;
}

Kit::Component* Kit::Component::hide()
{
    _isDisplay = false;
    return this;
}

Kit::Component* Kit::Component::toggleDisplay()
{
    _isDisplay = !_isDisplay;
    return this;
}

bool Kit::Component::display() const
{
    return _isDisplay;
}

SDL_Renderer* Kit::Component::renderer() const
{
    return _renderer;
}

SDL_Texture* Kit::Component::innerTexture() const
{
    return _innerTexture;
}

SDL_Texture* Kit::Component::outerTexture() const
{
    return _outerTexture;
}

Kit::Component* Kit::Component::parent() const
{
    return _parent;
}

const Kit::vector<Kit::Component*> &Kit::Component::childs() const
{
    return _childrens;
}

Kit::Window* Kit::Component::window() const
{
    return _window;
}

bool Kit::Component::isChildrenObject(Component* obj) const
{
    bool is_child = this == obj;

    if (!_childrens.empty() && !is_child)
    {
        for (auto &children : _childrens)
        {
            is_child |= children->isChildrenObject(obj);
        }
    }

    return is_child;
}

bool Kit::Component::isParentObject(Component* obj) const
{
    bool is_parent = this == obj;

    if (_parent != nullptr && !is_parent)
    {
        is_parent |= _parent->isParentObject(obj);
    }

    return is_parent;
}

Kit::Component* Kit::Component::append(Component* component)
{
    if (component == nullptr)
        return nullptr;


    component->_parent = this;

    this->_childrens.push_back(component);


    setupParentWindow();

    return component;
}

Kit::Component* Kit::Component::append(const vector<Component*> &components)
{
    for (auto &component : components)
    {
        append(component);
    }

    return this;
}

bool Kit::Component::onHover(const Point &point)
{
    return _isDisplay && point.in(_outerSize);
}

Kit::Component* Kit::Component::onComponentHover(Point point)
{
    if (point.in(_innerSize))
    {
        point.dy(_verticalScroll->_nowValue);
        point.dx(_horizontalScroll->_nowValue);

        for (int i = _childrens.size() - 1; i >= 0; i--)
        {
            auto &children = _childrens[i];

            if (children->onHover(point))
            {
                // adjust coord
                point = point - children->outerSize().start;

                return children->onComponentHover(point);
            }
        }
    }

    return this;
}

const Kit::string &Kit::Component::id() const
{
    return _id;
}

void Kit::Component::addEventListener(const string &action, eventCallback callback_function)
{
    _eventListeners[action] = std::move(callback_function);
}

void Kit::Component::removeEventListener(const string &action)
{
    _eventListeners[action] = Component::_emptyCallback;
}

void Kit::Component::callEventListener(const string &action, Event* e)
{
    if (_eventListeners.find(action) != _eventListeners.end())
    {
        _eventListeners[action](this, e);
    }
    else
    {
        cout << "Error: The transmitted event cannot be triggered because it has not been set!" << endl;
    }
}

Kit::map<Kit::string, void*> &Kit::Component::userData()
{
    return _userData;
}

void Kit::Component::addUserData(const string &key, void* data)
{
    _userData.insert(make_pair(key, data));
}

void* Kit::Component::userData(const string &key)
{
    return _userData[key];
}

bool Kit::Component::isHovered()
{
    return _isHovered;
}

bool Kit::Component::isActive()
{
    return _isActive;
}

bool Kit::Component::hasClass(const string &className) const
{
    return _classes.find(className) != -1;
}

Kit::Component* Kit::Component::removeClass(const string &className)
{
    int indexStart = _classes.find(className);

    if (indexStart == -1)
    {
        cout << "Classname " << className << " not found!" << endl;
        return this;
    }

    _classes.erase(_classes.begin() + indexStart, _classes.begin() + indexStart + className.length());


    _window->handleStyles();
    _window->render();
    return this;
}

Kit::Component* Kit::Component::addClass(const string &className)
{
    _classes += " " + className;

    _window->handleStyles();
    _window->render();
    return this;
}

Kit::Component* Kit::Component::toggleClass(const string &className)
{
    if (hasClass(className))
    {
        removeClass(className);
    }
    else
    {
        addClass(className);
    }

    return this;
}

const std::string &Kit::Component::classes()
{
    return _classes;
}

void Kit::Component::classes(const std::string& newClasses)
{
    this->_classes = newClasses;
}

void Kit::Component::setText(const string &text)
{
    if (_text != nullptr)
    {
        _text->setText(text);
        return;
    }

    this->_text_temp = text;
}

bool Kit::Component::isVerticalScrollable() const
{
    return _verticalScrollable;
}

bool Kit::Component::isHorizontalScrollable() const
{
    return _horizontalScrollable;
}

void Kit::Component::include(const string &path)
{
    if (_css_component == nullptr)
        _css_component = new CSS::css(path);

    _css_component->open(path);
}


void Kit::Component::outerWidth(int value)
{
    _outerSize.w(value);
}

void Kit::Component::outerHeight(int value)
{
    _outerSize.h(value);
}

void Kit::Component::outerLeft(int value)
{
    _outerSize.x(value);
}

void Kit::Component::outerTop(int value)
{
    _outerSize.y(value);
}

void Kit::Component::getFocus()
{
    cout << _id << " getFocus" << endl;

    this->_isFocused = true;
}

void Kit::Component::loseFocus()
{
    cout << _id << " loseFocus" << endl;

    this->_isFocused = false;
}


