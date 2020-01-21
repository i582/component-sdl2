#include "component.h"
#include "window/window.h"

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
        : Component(id, {0, 0, 0, 0}, classes, childrens)
{
}

Kit::Component::Component(const string& id, const Rect& size, const string& classes,
                          const vector<Component*>& childrens)
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
    for (auto& children : childrens)
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


    /** Beta Extended Text */
    this->_text_extended = nullptr;
    this->_is_one_line_text = false;
    this->_withExtendedText = false;


    /** Ignore Some Event */
    this->_ignoreEvents = false;
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

void Kit::Component::setupLifecycleHooks()
{
    _lifecycleHooks["componentWillMount"] = Component::_emptyLifecycleHook; // время сразу после создания компонента
    _lifecycleHooks["componentWillUnmount"] = Component::_emptyLifecycleHook; // время перед удалением компонента

    _lifecycleHooks["allComponentsDidConfigured"] = Component::_emptyLifecycleHook; // вызывается у каждого компонента,
                                                                                    // в момент когда все копоненты настроены
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

    for (auto& children : _childrens)
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

    for (auto& children : _childrens)
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


    for (auto& children : _childrens)
    {
        children->computeSize();
    }
}

void Kit::Component::computeChildrenSize()
{
    for (auto& children : _childrens)
    {
        children->computeChildrenSize();
    }


    SimpleSize childrenSize = {0, 0};

    for (auto& children : _childrens)
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



        // setup horizontal scroll
        const int scrollBodyHeight = 15;

        const int scrollSliderHeight = scrollBodyHeight - 6;
        const int scrollSliderPositionY = (scrollBodyHeight - scrollSliderHeight) / 2;

        const SimpleRect scrollBody =
        {
        /* x */ _innerSize.x(),
        /* y */ _innerSize.y() + _innerSize.h() - scrollBodyHeight,
        /* w */ _innerSize.w(),
        /* h */  scrollBodyHeight
        };

        const SimpleRect scrollSlider =
        {
        /* x */ 0, // auto
        /* y */ scrollSliderPositionY,
        /* w */ 0, // auto
        /* h */ scrollSliderHeight
        };

        const int maxValue = newTextureSize.w() - _innerSize.w();
        const int nowValue = 0;
        const double aspectRatio = _innerSize.w() / (double) newTextureSize.w();
        const double nowPosition = 0;

        _horizontalScroll->setup(
            scrollBody,
            scrollSlider,
            maxValue,
            nowValue,
            aspectRatio,
            nowPosition
        );

        _horizontalScrollable = true;


        _innerSize.dh(-scrollBodyHeight);

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



        // setup vertical scroll
        const int scrollBodyWidth = 15;

        const int scrollSliderWidth = scrollBodyWidth - 6;
        const int scrollSliderPositionX = (scrollBodyWidth - scrollSliderWidth) / 2;

        const SimpleRect scrollBody =
        {
        /* x */ _innerSize.x() + _innerSize.w() - scrollBodyWidth,
        /* y */ _innerSize.y(),
        /* w */  scrollBodyWidth,
        /* h */ _innerSize.h()
        };

        const SimpleRect scrollSlider =
        {
        /* x */ scrollSliderPositionX,
        /* y */ 0, // auto
        /* w */ scrollSliderWidth,
        /* h */ 0 // auto
        };

        const int maxValue = newTextureSize.h() - _innerSize.h();
        const int nowValue = 0;
        const double aspectRatio = _innerSize.h() / (double) newTextureSize.h();
        const double nowPosition = 0;

        _verticalScroll->setup(
            scrollBody,
            scrollSlider,
            maxValue,
            nowValue,
            aspectRatio,
            nowPosition
        );

        _verticalScrollable = true;


        _innerSize.dw(-scrollBodyWidth);

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
    this->_image = new image(this);

    for (auto& children : _childrens)
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


    for (auto& children : _childrens)
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

    for (auto& children : _childrens)
    {
        children->setupText();
    }
}

void Kit::Component::setupScrolls()
{
    this->_verticalScroll = new vertical_scroll(this);

    this->_horizontalScroll = new horizontal_scroll(this);


    for (auto& children : _childrens)
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


        _text_extended = new text(this, _text_temp, {0, 0}, _fontNormal, fontSize, textColor, lineHeight);

        _text_extended->_is_one_line_text = _is_one_line_text;
    }

    for (auto& children : _childrens)
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

    for (auto& children : _childrens)
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


    for (auto& children : _childrens)
    {
        children->setupChildrenPosition();
    }
}

void Kit::Component::adjustMousePoint(Point& p)
{
    if (_parent == nullptr)
        return;

    p = p - _outerSize.start;
    p = p - _innerSize.start;

    _parent->adjustMousePoint(p);
}

void Kit::Component::handleStyles()
{
    if (_css_component == nullptr)
        return;

    auto styles = _css_component->getStyles();


    // получаем все классы текущего компонента
    auto classnames = Utils::split(this->_classes, ' ');

    auto is_have_styles = false;

    // создаем временный блок хранения стилей с стандартными стилями
    CSS::css_block ready_block(this->_id, true);


    for (auto& classname : *classnames)
    {
        // если в наших стилях есть стиль для текущего класса-идентификатора
        if (styles.find(classname) != styles.end())
        {
            auto style = styles[classname];

            // добавляем их в временный блок
            ready_block.mergeWith(style);

            // добавляем все в основной блок стилей
            this->_cssBlock.mergeWith(ready_block);

            is_have_styles = true;
        }
    }

    // если стили не были найдены то просто добавляем временные стили в блок стилей
    if (!is_have_styles)
    {
        this->_cssBlock.mergeWith(ready_block);
    }

    delete classnames;


    // проходим по стилям и если там если стили для дочерних компонентов то устанавливаем им
    // данные стили в временное хранилище css, что бы при следующем проходе установить их как показано выше
    for (auto& [block_class, block] : styles)
    {
        for (auto& children : _childrens)
        {
            auto children_classnames = Utils::split(children->_classes, ' ');

            for (auto& children_classname : *children_classnames)
            {
                if (children_classname == block_class)
                {
                    children->_css_component = this->_css_component;
                }
            }

            delete children_classnames;
        }
    }


    for (auto& children : _childrens)
    {
        children->handleStyles();
    }
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


    const auto& borderRadius_1 = blockState->get<int>("border-radius-top-left");
    const auto& borderRadius_2 = blockState->get<int>("border-radius-top-right");
    const auto& borderRadius_3 = blockState->get<int>("border-radius-bottom-left");
    const auto& borderRadius_4 = blockState->get<int>("border-radius-bottom-right");




    const bool outline = blockState->get<Color>("outline") != Color(0);

    if (outline)
    {
        const auto outlineColor = blockState->get<Color>("outline");

        const SimpleRect _outlineRectSize = {0, 0, _innerSize.w(), _innerSize.h()};
        Draw::roundedRect(_renderer, _outlineRectSize, borderRadius_1, borderRadius_2, borderRadius_3, borderRadius_4, outlineColor);
    }

    const SimpleRect _innerRectSize = {outline, outline, _innerSize.w() - 2 * outline, _innerSize.h() - 2 * outline};
    Draw::roundedRect(_renderer, _innerRectSize,
            borderRadius_1,
            borderRadius_2,
            borderRadius_3,
            borderRadius_4,
            backgroundColor
    );




    if (this->_image != nullptr)
    {
        const string& pathToImage = blockState->get<string>("background-image");
        _image->path(pathToImage);


        const string& x_shift = blockState->get<string>("background-position-x");
        const string& y_shift = blockState->get<string>("background-position-y");
        _image->position({x_shift, y_shift});


        const string& imageWidth = blockState->get<string>("background-size");
        _image->size({imageWidth, "0px"}, true);


        _image->render();
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


    if (_text_extended != nullptr && _withExtendedText)
    {
        _text_extended->useFont(blockFont);
        _text_extended->useColor(blockState->get<Color>("color"));
        _text_extended->fontSize(blockState->get<int>("font-size"));
        _text_extended->lineHeight(blockState->get<double>("line-height"));
        _text_extended->textHorizontalAlign(blockState->get<string>("text-align"));
        _text_extended->textVerticalAlign(blockState->get<string>("vertical-align"));

//        _text_extended->setTextBlockMargin("top", blockState->get<int>("margin-top"));
//        _text_extended->setTextBlockMargin("bottom", blockState->get<int>("margin-bottom"));
//        _text_extended->setTextBlockMargin("left", blockState->get<int>("margin-left"));
//        _text_extended->setTextBlockMargin("right", blockState->get<int>("margin-right"));

        _text_extended->focus(_isFocused);


        _text_extended->render();


    }







    for (auto& children : _childrens)
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
    Draw::roundedRect(_renderer, _innerRectBorderSize,
            borderRadius_1 + leftSize,
            borderRadius_2 + leftSize,
            borderRadius_3 + leftSize,
            borderRadius_4 + leftSize,
            borderColor
    );


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
        copy.y(this->_verticalScroll->_nowValue);
        copy.x(this->_horizontalScroll->_nowValue);
    }


    const SDL_Rect copy_sdl = copy.toSdlRect();
    const SDL_Rect innerSize_sdl = _innerSize.toSdlRect();

    SDL_RenderCopy(_renderer, _innerTexture, &copy_sdl, &innerSize_sdl);



    if (_verticalScrollable)
    {
        _verticalScroll->render();
    }

    if (_horizontalScrollable)
    {
        _horizontalScroll->render();
    }



    SDL_Texture* parentTexture = nullptr;
    if (_parent != nullptr)
    {
        parentTexture = _parent->innerTexture();
    }

    SDL_SetRenderTarget(_renderer, parentTexture);


    const SDL_Rect outerSize_sdl = _outerSize.toSdlRect();
    SDL_RenderCopy(_renderer, _outerTexture, nullptr, &outerSize_sdl);
}

void Kit::Component::mouseButtonDown(Event* e_)
{
    if (!_isDisplay)
        return;


    cout << _id << " mouseButtonDown" << endl;

    Point mouseP(e_->motion.x, e_->motion.y);

    adjustMousePoint(mouseP);


    if (_verticalScroll->onSliderHover(mouseP))
    {
        cout << "_verticalScroll slider" << endl;

        cout << "_verticalScroll active TRUE" << endl;
        _isVerticalScrollActive = true;

        return;
    }

    if (_horizontalScroll->onSliderHover(mouseP))
    {
        cout << "_horizontalScroll slider" << endl;

        cout << "_horizontalScroll active TRUE" << endl;
        _isHorizontalScrollActive = true;

        return;
    }


    _eventListeners["click"](this, e_);
    _eventListeners["onmousedown"](this, e_);

    _isActive = true;

    if (_withExtendedText)
    {
        _text_extended->mouseButtonDown(e_, mouseP);
        //_extended_text->mouseButtonDown(e_, mouseP);
        return;
    }

    if (_parent != nullptr)
    {
        _parent->mouseButtonDown(e_);
    }
}

void Kit::Component::mouseButtonUp(Event* e_)
{
    if (!_isDisplay)
        return;

    cout << _id << " mouseButtonUp" << endl;

    _eventListeners["onmouseup"](this, e_);


    _isVerticalScrollActive = false;
    _isHorizontalScrollActive = false;
    _isActive = false;


    if (_withExtendedText)
    {
        Point mouseP(e_->motion.x, e_->motion.y);
        adjustMousePoint(mouseP);

        _text_extended->mouseButtonUp(e_, mouseP);
        //_extended_text->mouseButtonUp(e_, mouseP);
        return;
    }

    if (_parent != nullptr)
    {
        _parent->mouseButtonUp(e_);
    }
}

void Kit::Component::mouseMotion(Event* e_)
{
    if (!_isDisplay)
        return;

    if (_isVerticalScrollActive)
    {
        _verticalScroll->shift(e_->motion.yrel * (_childrensSize.h() / _innerSize.h()));
        return;
    }

    if (_isHorizontalScrollActive)
    {
        _horizontalScroll->shift(e_->motion.xrel * (_childrensSize.w() / _innerSize.w()));
        return;
    }

    if (!_isEnterInComponent)
    {
        _eventListeners["onmouseover"](this, e_);
        _isEnterInComponent = true;
    }

    if (_hoverComponent != this)
    {
        if (_hoverComponent != nullptr && !_hoverComponent->isChildrenObject(this))
        {
            _hoverComponent->mouseOut(e_);
        }

        _hoverComponent = this;
    }


    if (_withExtendedText)
    {
        Point mouseP(e_->motion.x, e_->motion.y);
        adjustMousePoint(mouseP);

        _text_extended->mouseMotion(e_, mouseP);
    }

    _isHovered = true;

    _eventListeners["mousemotion"](this, e_);
    _eventListeners["hover"](this, e_);
}

void Kit::Component::mouseOut(Event* e_)
{
    if (!_isDisplay)
        return;

    _eventListeners["onmouseout"](this, e_);

    _isHovered = false;
    _isEnterInComponent = false;
}

void Kit::Component::mouseScroll(Event* e_, int scrollDirection)
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
                firstScrollableParent->mouseScroll(e_, scrollDirection);
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
                firstScrollableParent->mouseScroll(e_, scrollDirection);
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

void Kit::Component::keyDown(Event* e_)
{
    if (_withExtendedText)
    {
        _text_extended->keyDown(e_);
    }
}

void Kit::Component::textInput(Event* e_)
{
    if (_withExtendedText)
    {
        _text_extended->textInput(e_);
    }
}

void Kit::Component::setupComponents()
{
    setupChildrenRenderer();

    setupScrolls();

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

const Kit::Rect& Kit::Component::size() const
{
    return _innerSize;
}

const Kit::Rect& Kit::Component::outerSize() const
{
    return _outerSize;
}

const Kit::Rect& Kit::Component::innerSize() const
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

const Kit::vector<Kit::Component*>& Kit::Component::childrens() const
{
    return _childrens;
}

Kit::window* Kit::Component::parentWindow() const
{
    return _window;
}

Kit::Component* Kit::Component::children(const string& id_)
{
    for (auto& children : _childrens)
    {
        if (children->_id == id_)
        {
            return children;
            break;
        }
    }

    throw std::logic_error("Children component with id: " + id_ + " not found!");
}

Kit::Component* Kit::Component::children(size_t number_)
{
    if (number_ > _childrens.size())
        throw std::logic_error("The transmitted number is greater than the number of child components!");

    return _childrens[number_];
}

bool Kit::Component::isChildrenObject(Component* obj) const
{
    bool is_child = this == obj;

    if (!_childrens.empty() && !is_child)
    {
        for (auto& children : _childrens)
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

    _childrens.push_back(component);


    setupParentWindow();

    return component;
}

Kit::Component* Kit::Component::append(const vector<Component*>& components)
{
    for (auto& component : components)
    {
        append(component);
    }

    return this;
}

bool Kit::Component::onHover(const Point& point)
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

const Kit::string& Kit::Component::id() const
{
    return _id;
}

Kit::Component* Kit::Component::addEventListener(const string& action, eventCallback callback_function)
{
    _eventListeners[action] = std::move(callback_function);

    return this;
}

Kit::Component* Kit::Component::removeEventListener(const string& action)
{
    _eventListeners[action] = Component::_emptyCallback;

    return this;
}

Kit::Component* Kit::Component::callEventListener(const string& action, Event* e_)
{
    if (_eventListeners.find(action) != _eventListeners.end())
    {
        _eventListeners[action](this, e_);
    }
    else
    {
        cout << "Error: The transmitted event cannot be triggered because it has not been set!" << endl;
    }

    return this;
}

Kit::map<Kit::string, std::any>& Kit::Component::userData()
{
    return _userData;
}

Kit::Component* Kit::Component::addUserData(const string& key, const std::any& data)
{
    _userData.insert(make_pair(key, data));

    return this;
}

Kit::Component* Kit::Component::setUserData(const string& key, const std::any& data)
{
    if (_userData.find(key) == _userData.end())
    {
        throw std::logic_error("User information for " + key + " does not exist.");
    }

    _userData[key] = data;

    return this;
}

std::any Kit::Component::userData(const string& key)
{
    if (_userData.find(key) == _userData.end())
    {
        throw std::logic_error("User information for " + key + " does not exist.");
    }

    return _userData[key];
}

bool Kit::Component::hasClass(const string& className) const
{
    return _classes.find(className) != -1;
}

Kit::Component* Kit::Component::removeClass(const string& className)
{
    int indexStart = _classes.find(className);

    if (indexStart == -1)
    {
        cout << "Classname " << className << " not found!" << endl;
        return this;
    }

    _classes.erase(_classes.begin() + indexStart, _classes.begin() + indexStart + className.length());


    if (_window != nullptr)
    {
        _window->handleStyles();
    }

    return this;
}

Kit::Component* Kit::Component::addClass(const string& className)
{
    _classes += " " + className;

    if (_window != nullptr)
    {
        _window->handleStyles();
    }

    return this;
}

Kit::Component* Kit::Component::toggleClass(const string& className)
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

Kit::Component* Kit::Component::setText(const string& text)
{
    if (_text != nullptr)
    {
        _text->setText(text);
        return this;
    }

    if (_text_extended != nullptr && _withExtendedText)
    {
        _text_extended->withText(text);
        return this;
    }

    this->_text_temp = text;

    return this;
}

Kit::Component* Kit::Component::oneLineText()
{
    _is_one_line_text = true;
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

Kit::Component* Kit::Component::style(const string& path)
{
    if (_css_component == nullptr)
        _css_component = new CSS::css(path);

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

Kit::Component* Kit::Component::getFocus(SDL_Event* e_)
{
    cout << _id << " getFocus" << endl;


    _eventListeners["focus"](this, e_);
    this->_isFocused = true;

    return this;
}

Kit::Component* Kit::Component::loseFocus(SDL_Event* e_)
{
    cout << _id << " loseFocus" << endl;

    _eventListeners["focusout"](this, e_);
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
        return charset[rand() % max_index];
    };
    std::string str(length, 0);
    std::generate_n(str.begin(), length, rand_char);
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
