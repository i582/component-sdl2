#include "component.h"
#include "../window/window.h"

Kit::Component* Kit::Component::_hoverComponent = nullptr;


Kit::Component* Kit::Component::create(const string& id, const string& classes, const vector<Component*>& childrens)
{
    return new Component(id, classes, childrens);
}

Kit::Component* Kit::Component::create(const std::string& classes, const std::vector<Kit::Component*>& childrens)
{
    return new Component("", classes, childrens);
}

Kit::Component* Kit::Component::create(Component* component)
{
    return component;
}


Kit::Component::Component(const std::string& id, const string& classes, const vector<Component*>& childrens)
: Component(id, {0, 0, 0, 0}, classes, childrens) {}

Kit::Component::Component(const string &id, const Rect& size, const string &classes, const vector<Component*> &childrens)
{
    /** Sizes */
    this->_innerSize = size;
    this->_outerSize = size;
    this->_isConstructSize = !size.empty();

    this->_autoWidth = false;
    this->_autoHeight = false;


    /** Identifiers */
    this->_id = id;
    this->checkID();

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
    this->_verticalScroll = nullptr;
    this->_verticalScrollable = false;
    this->_isVerticalScrollActive = false;


    /** Horizontal Scroll */
    this->_horizontalScroll = nullptr;
    this->_horizontalScrollable = false;
    this->_isHorizontalScrollActive = false;

    this->_needRenderScroll = true;


    /** Image */
    this->_image = nullptr;


    /** CSS component */
    this->_css_component = nullptr;


    /** Extended text */
    this->_extended_text = nullptr;
    this->_withExtendedText = false;

    /** Ignore Some Event */
    this-> _ignoreEvents = false;
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
    _eventListeners["focus"] = Component::_emptyCallback;
    _eventListeners["focusout"] = Component::_emptyCallback;
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
            normalBlock.get<int>("border-bottom-size"),
            hoverBlock.get<int>("border-bottom-size"),
            activeBlock.get<int>("border-bottom-size")
    );
    const auto leftSize = Utils::max_of(
            normalBlock.get<int>("border-left-size"),
            hoverBlock.get<int>("border-left-size"),
            activeBlock.get<int>("border-left-size")
    );
    const auto rightSize = Utils::max_of(
            normalBlock.get<int>("border-right-size"),
            hoverBlock.get<int>("border-right-size"),
            activeBlock.get<int>("border-right-size")
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

void Kit::Component::setupScrolls()
{
    this->_verticalScroll = new VerticalScroll(_renderer, {0, 0, 15, 0}, 0, 1);

    this->_horizontalScroll = new HorizontalScroll(_renderer, {0, 0, 0, 15}, 0, 1);



    for (auto &children : _childrens)
    {
        children->setupScrolls();
    }
}

void Kit::Component::setupExtendedText()
{
    if (_withExtendedText)
    {
        auto fontSize = _cssBlock.normal().get<int>("font-size");
        auto textColor = _cssBlock.normal().get<Color>("color");
        auto lineHeight = _cssBlock.normal().get<double>("line-height");

        this->_extended_text = new Text2(this, _text_temp,
                                         {0, 0, _innerSize.w(), _innerSize.h()},
                                         _fontNormal, fontSize, textColor, lineHeight, true);
    }

    for (auto &children : _childrens)
    {
        children->setupExtendedText();
    }
}

void Kit::Component::checkID()
{
    if (this->_id.empty())
    {
        this->_id = Component::generateRandomString();
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
        else if (display_type == "inline-block")
        {
            auto& first_children = _childrens[0];

            first_children->outerLeft(0);
            first_children->outerTop(0);


            for (size_t i = 1; i < _childrens.size(); ++i)
            {
                auto& children = _childrens[i];
                auto& children_prev = _childrens[i - 1];

                const int new_x_position = children_prev->outerSize().w() + children_prev->outerSize().x();

                children->outerTop(0);
                children->outerLeft(new_x_position);
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


    const bool outline = blockState->get<Color>("outline") != Color(0);

    if (outline)
    {
        const auto outlineColor = blockState->get<Color>("outline");

        const SimpleRect _outlineRectSize = { 0, 0, _innerSize.w(), _innerSize.h() };
        Draw::roundedRect(_renderer, _outlineRectSize, borderRadius, outlineColor);
    }

    const SimpleRect _innerRectSize = { outline, outline, _innerSize.w() - 2*outline, _innerSize.h() - 2*outline };
    Draw::roundedRect(_renderer, _innerRectSize, borderRadius, backgroundColor);





    if (this->_image != nullptr)
    {

        this->_image->setPath(blockState->get<string>("background-image"));


        const int& x_shift = blockState->get<int>("background-position-x");
        const int& y_shift = blockState->get<int>("background-position-y");
        this->_image->setImageShift({x_shift, y_shift});


        const string& newSize = blockState->get<string>("background-size");
        this->_image->setImageWidth(newSize);


        this->_image->render();
    }


    if (!_withExtendedText)
    {

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

    }


    if (this->_extended_text != nullptr && _withExtendedText)
    {
        this->_extended_text->setFont(blockFont);
        this->_extended_text->setColor(blockState->get<Color>("color"));
        this->_extended_text->setFontSize(blockState->get<int>("font-size"));
        this->_extended_text->setLineHeight(blockState->get<double>("line-height"));
        this->_extended_text->setTextAlign(blockState->get<string>("text-align"));
        this->_extended_text->setTextBlockVerticalAlign(blockState->get<string>("vertical-align"));

        this->_extended_text->setTextBlockMargin("top", blockState->get<int>("margin-top"));
        this->_extended_text->setTextBlockMargin("bottom", blockState->get<int>("margin-bottom"));
        this->_extended_text->setTextBlockMargin("left", blockState->get<int>("margin-left"));
        this->_extended_text->setTextBlockMargin("right", blockState->get<int>("margin-right"));

        this->_extended_text->setFocus(_isFocused);

        this->_extended_text->render();
    }


    for (auto &children : _childrens)
    {
        children->render();
    }


    SDL_SetRenderTarget(_renderer, _outerTexture);


    const auto& topSize = blockState->get<int>("border-top-size");
    const auto& bottomSize = blockState->get<int>("border-bottom-size");
    const auto& leftSize = blockState->get<int>("border-left-size");
    const auto& rightSize = blockState->get<int>("border-right-size");


    const SDL_Rect _innerRectBorderSize =
    {
            _innerSize.x() - leftSize,
            _innerSize.y() - topSize,
            _innerSize.w() + leftSize + rightSize,
            _innerSize.h() + topSize + bottomSize
    };
    Draw::roundedRect(_renderer, _innerRectBorderSize, borderRadius, borderColor);





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


    cout << _id << " mouseButtonDown" << endl;

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

    if (_withExtendedText)
    {
        _extended_text->mouseButtonDown(e, mouseP);
        return;
    }

    if (_parent != nullptr)
    {
        _parent->mouseButtonDown(e);
    }
}

void Kit::Component::mouseButtonUp(Event* e)
{
    if (!_isDisplay)
        return;

    cout << _id << " mouseButtonUp" << endl;

    _eventListeners["onmouseup"](this, e);


    _isVerticalScrollActive = false;
    _isHorizontalScrollActive = false;
    _isActive = false;


    if (_withExtendedText)
    {
        Point mouseP(e->motion.x, e->motion.y);
        adjustMousePoint(mouseP);

        _extended_text->mouseButtonUp(e, mouseP);
        return;
    }

    if (_parent != nullptr)
    {
        _parent->mouseButtonUp(e);
    }
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


    if (_withExtendedText)
    {
        Point mouseP(e->motion.x, e->motion.y);
        adjustMousePoint(mouseP);

        _extended_text->mouseMotion(e, mouseP);
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

void Kit::Component::keyDown(Event* e)
{
    if (_withExtendedText)
    {
        _extended_text->keyDown(e);
    }
}

void Kit::Component::textInput(Event* e)
{
    if (_withExtendedText)
    {
        _extended_text->textInput(e);
    }
}

void Kit::Component::setupComponents()
{
    setupScrolls();
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
            auto& children = _childrens[i];

            if (children->onHover(point) && !children->isIgnoreEvents())
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

Kit::Component* Kit::Component::addEventListener(const string &action, eventCallback callback_function)
{
    _eventListeners[action] = std::move(callback_function);

    return this;
}

Kit::Component* Kit::Component::removeEventListener(const string &action)
{
    _eventListeners[action] = Component::_emptyCallback;

    return this;
}

Kit::Component* Kit::Component::callEventListener(const string &action, Event* e)
{
    if (_eventListeners.find(action) != _eventListeners.end())
    {
        _eventListeners[action](this, e);
    }
    else
    {
        cout << "Error: The transmitted event cannot be triggered because it has not been set!" << endl;
    }

    return this;
}

Kit::map<Kit::string, std::any> &Kit::Component::userData()
{
    return _userData;
}

Kit::Component* Kit::Component::addUserData(const string &key, const std::any& data)
{
    _userData.insert(make_pair(key, data));

    return this;
}

std::any Kit::Component::userData(const string &key)
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

const std::string& Kit::Component::classes()
{
    return _classes;
}

Kit::Component* Kit::Component::classes(const std::string& newClasses)
{
    this->_classes = newClasses;

    return this;
}

Kit::Component* Kit::Component::setText(const string &text)
{
    if (_text != nullptr)
    {
        _text->setText(text);
        return this;
    }

    if (_extended_text != nullptr && _withExtendedText)
    {
        _extended_text->setText(text);
        return this;
    }

    this->_text_temp = text;

    return this;
}

bool Kit::Component::isVerticalScrollable() const
{
    return _verticalScrollable;
}

bool Kit::Component::isHorizontalScrollable() const
{
    return _horizontalScrollable;
}

Kit::Component* Kit::Component::include(const string &path)
{
    if (_css_component == nullptr)
        _css_component = new CSS::css(path);

    _css_component->open(path);

    return this;
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

Kit::Component* Kit::Component::getFocus(SDL_Event* e)
{
    cout << _id << " getFocus" << endl;


    _eventListeners["focus"](this, e);
    this->_isFocused = true;

    return this;
}

Kit::Component* Kit::Component::loseFocus(SDL_Event* e)
{
    cout << _id << " loseFocus" << endl;

    _eventListeners["focusout"](this, e);
    this->_isFocused = false;

    return this;
}

std::string Kit::Component::generateRandomString()
{
    srand(clock());
    size_t length = rand() % 7 + 8;

    auto rand_char = []() -> char
    {
        const char charset[] =
                "0123456789"
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "abcdefghijklmnopqrstuvwxyz";

        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length, 0);
    std::generate_n( str.begin(), length, rand_char );
    return str;
}

Kit::Component* Kit::Component::ignoreEvents()
{
    this->_ignoreEvents = true;

    return this;
}

Kit::Component* Kit::Component::noIgnoreEvents()
{
    this->_ignoreEvents = false;

    return this;
}

bool Kit::Component::isIgnoreEvents()
{
    return this->_ignoreEvents;
}

Kit::Component* Kit::Component::useExtendedText()
{
    this->_withExtendedText = true;

    return this;
}

Kit::Component* Kit::Component::unuseExtendedText()
{
    this->_withExtendedText = false;

    return this;
}
