#include "component.h"
#include "../window/window.h"

Kit::Component* Kit::Component::_hoverComponent = nullptr;

Kit::Component::Component(string id, Rect size, string classes) : Component(id, size, classes, {}) {}

Kit::Component::Component(string id, Rect size, string classes, vector<Component*> childrens)
{
	/** Sizes */
	this->_innerSize = size;
	this->_outerSize = size;


	/** Identifiers */
	this->_id = id;
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
	this->setupEventListeners(); // TO DO 


	/** User data */
	this->_userData;


	/** State */
	this->_isHovered = false;
	this->_isActive = false;



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


	/** Scroll */
	this->_verticalScroll = new VerticalScroll(_renderer, { 0, 0, 15, 0 }, 0, 1);
	this->_horizontalScroll = new HorizontalScroll(_renderer, { 0, 0, 0, 15 }, 0, 1);

	this->_verticalScrollable = false;
	this->_horizontalScrollable = false;

	this->_isVerticalScrollActive = false;
	this->_isHorizontalScrollActive = false;

	this->_needRenderScroll = true;


	/** Image */
	this->_image = nullptr;


	/** CSS component */
	CSS::css* _css_component = nullptr;
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


Kit::Component* Kit::Component::getFirstScrollableParent()
{
	if (this->_parent != nullptr)
	{
		if (this->_parent->isVerticalScrollable() || this->_parent->isHorizontalScrollable())
		{
			return this->_parent;
		}
		else
		{
			return this->_parent->getFirstScrollableParent();
		}
	}

	return nullptr;
}

CSS::css* Kit::Component::getComponentStyles()
{
	return this->_css_component;
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
		Rect parentSize = _parent->size();
		_innerSize.calc(parentSize);
		_outerSize.calc(parentSize);
	}




	/**
	 *  Рассчитываем размеры с учетом обводки
	 */

	/** Получаем максимальный размер обводки для каждой из сторон */
	int topSize = std::_Max_value(std::_Max_value(_cssBlock.normal().get<int>("border-top-size"),
		_cssBlock.hover().get<int>("border-top-size")), _cssBlock.active().get<int>("border-top-size"));
	int bottomSize = std::_Max_value(std::_Max_value(_cssBlock.normal().get<int>("border-bottom-size"),
		_cssBlock.hover().get<int>("border-bottom-size")), _cssBlock.active().get<int>("border-bottom-size"));
	int leftSize = std::_Max_value(std::_Max_value(_cssBlock.normal().get<int>("border-left-size"),
		_cssBlock.hover().get<int>("border-left-size")), _cssBlock.active().get<int>("border-left-size"));
	int rightSize = std::_Max_value(std::_Max_value(_cssBlock.normal().get<int>("border-right-size"),
		_cssBlock.hover().get<int>("border-right-size")), _cssBlock.active().get<int>("border-right-size"));

	/** Корректируем внешний размер */
	_outerSize.size.dw(leftSize + rightSize);
	_outerSize.size.dh(topSize + bottomSize);

	/** Корректируем положение внешнего размера */
	_outerSize.start.dx(-leftSize);
	_outerSize.start.dy(-topSize);

	/** Корректируем положение внутреннего размера, так чтобы
	визуально объект находился в изначальных координатах */
	_innerSize.start.x(leftSize);
	_innerSize.start.y(topSize);


	/** Пересоздаем текстуру */
	SDL_DestroyTexture(this->_outerTexture);
	this->_outerTexture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _outerSize.w(), _outerSize.h());
	SDL_SetTextureBlendMode(_outerTexture, SDL_BLENDMODE_BLEND);

	/** Пересоздаем текстуру */
	SDL_DestroyTexture(this->_innerTexture);
	this->_innerTexture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _innerSize.w(), _innerSize.h());
	SDL_SetTextureBlendMode(_innerTexture, SDL_BLENDMODE_BLEND);



	for (auto& children : _childrens)
	{
		children->computeSize();
	}
}

void Kit::Component::computeChildrenSize()
{
	SimpleSize childrenSize;

	/** Находим размер прямоугольника в который можно уместить всех детей */
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

	_childrensSize = { 0, 0, childrenSize.w, childrenSize.h };


	Size newTextureSize = _innerSize.size;

	if (childrenSize.w > _innerSize.w())
	{
		newTextureSize.w(childrenSize.w);



		/*
		 * Setup scroll TO DO
		 */
		this->_horizontalScroll->_bodySize.x(_innerSize.x());
		this->_horizontalScroll->_bodySize.y(_innerSize.y() + _innerSize.h());

		this->_horizontalScroll->_bodySize.w(_innerSize.w());

		this->_horizontalScroll->_maxValue = newTextureSize.w() - _innerSize.w();

		this->_horizontalScroll->_relSizes = _innerSize.w() / (double)newTextureSize.w();
		this->_horizontalScroll->_renderer = _renderer;
		this->_horizontalScroll->init();

		_outerSize.dh(this->_horizontalScroll->_bodySize.h());

		SDL_DestroyTexture(this->_outerTexture);
		this->_outerTexture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _outerSize.w(), _outerSize.h());
		SDL_SetTextureBlendMode(_outerTexture, SDL_BLENDMODE_BLEND);

		this->_horizontalScroll->_parentTexture = this->_outerTexture;
		this->_horizontalScrollable = true;
	}

	if (childrenSize.h > _innerSize.h())
	{
		newTextureSize.h(childrenSize.h);



		/*
		 * Setup scroll TO DO
		 */
		this->_verticalScroll->_bodySize.x(_innerSize.x() + _innerSize.w());
		this->_verticalScroll->_bodySize.y(_innerSize.y());

		this->_verticalScroll->_bodySize.h(_innerSize.h());

		this->_verticalScroll->_maxValue = newTextureSize.h() - _innerSize.h();

		this->_verticalScroll->_relSizes = _innerSize.h() / (double)newTextureSize.h();
		this->_verticalScroll->_renderer = _renderer;
		this->_verticalScroll->init();

		_outerSize.dw(this->_verticalScroll->_bodySize.w());

		SDL_DestroyTexture(this->_outerTexture);
		this->_outerTexture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _outerSize.w(), _outerSize.h());
		SDL_SetTextureBlendMode(_outerTexture, SDL_BLENDMODE_BLEND);

		this->_verticalScroll->_parentTexture = this->_outerTexture;
		this->_verticalScrollable = true;
	}


	if (newTextureSize != _innerSize.size)
	{
		SDL_DestroyTexture(this->_innerTexture);
		this->_innerTexture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, newTextureSize.w(), newTextureSize.h());
		SDL_SetTextureBlendMode(_outerTexture, SDL_BLENDMODE_BLEND);
	}



	for (auto& children : _childrens)
	{
		children->computeChildrenSize();
	}
}

void Kit::Component::setupBackgroundImage()
{
	this->_image = new Image(this);

	for (auto& children : _childrens)
	{
		children->setupBackgroundImage();
	}
}

void Kit::Component::setupFont()
{
	string font_family = _cssBlock.normal().get<string>("font-family");
	string font_style = _cssBlock.normal().get<string>("font-style");
	int font_weight = _cssBlock.normal().get<int>("font-weight");
	
	this->_fontNormal = font(font_family, font_style, font_weight);


	font_family = _cssBlock.hover().get<string>("font-family");
	font_style = _cssBlock.hover().get<string>("font-style");
	font_weight = _cssBlock.hover().get<int>("font-weight");

	this->_fontHover = font(font_family, font_style, font_weight);
	

	font_family = _cssBlock.active().get<string>("font-family");
	font_style = _cssBlock.active().get<string>("font-style");
	font_weight = _cssBlock.active().get<int>("font-weight");

	this->_fontActive = font(font_family, font_style, font_weight);


	for (auto& children : _childrens)
	{
		children->setupFont();
	}
}

void Kit::Component::setupText()
{
	int font_size = _cssBlock.normal().get<int>("font-size");

	this->_text = new Text(this, _text_temp, { 0, 0, _innerSize.w(), _innerSize.h() }, _fontNormal, font_size, Color("#000000"));
	this->_text->setRenderer(this->_renderer);

	for (auto& children : _childrens)
	{
		children->setupText();
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

void Kit::Component::adjustMousePoint(Point& p)
{
	if (_parent == nullptr)
		return;

	p = p - _outerSize.start;

	_parent->adjustMousePoint(p);
}

void Kit::Component::render()
{
	if (!_isDisplay)
		return;

	SDL_SetRenderTarget(_renderer, _outerTexture);
	SDL_SetRenderDrawColor(_renderer, 0xff, 0xff, 0xff, 0x00);
	SDL_RenderClear(_renderer);

	SDL_SetRenderTarget(_renderer, _innerTexture);
	SDL_SetRenderDrawColor(_renderer, 0xff, 0xff, 0xff, 0x00);
	SDL_RenderClear(_renderer);


	CSS::css_block_state* blockState = &_cssBlock.normal();
	
	font blockFont = _fontNormal;

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


	Color _bk = blockState->get<Color>("background-color");
	Color _br = blockState->get<Color>("border-color");

	int borderRadius = blockState->get<int>("border-radius");

	roundedBoxColor(_renderer, 0, 0, _innerSize.w(), _innerSize.h(), borderRadius, _bk.color());



	if (this->_image != nullptr)
	{
		
		this->_image->setPath(blockState->get<string>("background-image"));

		const int& x_shift = blockState->get<int>("background-position-x");
		const int& y_shift = blockState->get<int>("background-position-y");

		this->_image->setImageShift({ x_shift, y_shift });

		const string& newSize = blockState->get<string>("background-size");

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



	for (auto& children : _childrens)
	{
		children->render();
	}


	SDL_SetRenderTarget(_renderer, _outerTexture);

	

	int topSize = blockState->get<int>("border-top-size");
	int bottomSize = blockState->get<int>("border-bottom-size");
	int leftSize = blockState->get<int>("border-left-size");
	int rightSize = blockState->get<int>("border-right-size");

	/** Рисуем обводку */
	roundedBoxColor(_renderer, _innerSize.x() - leftSize, _innerSize.y() - topSize,
		_innerSize.x() + _innerSize.w() - 1 + rightSize, _innerSize.y() + _innerSize.h() - 1 + bottomSize, borderRadius, _br.color());



	Rect copy = _innerSize;
	

	string overflow = blockState->get<string>("overflow");


	if (overflow == "hidden")
	{
		copy.y(0); copy.x(0);
		_needRenderScroll = false;
		_verticalScrollable = false;
	}
	else
	{
		copy.x(this->_horizontalScroll->_nowValue);
		copy.y(this->_verticalScroll->_nowValue);

		_verticalScrollable = true;
		_horizontalScrollable = true;
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
	SDL_RenderCopy(_renderer, _outerTexture, NULL, &_outerSize.toSdlRect());
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

	//cout << "scroll active FALSE" << endl;
	_isVerticalScrollActive = false;
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

	if (_isEnterInComponent == false)
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
	//SDL_SetCursor(style->hoverCursor());

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

	if (!_verticalScrollable)
	{
		Component* firstScrollableParent = getFirstScrollableParent();

		if (firstScrollableParent != nullptr)
		{
			firstScrollableParent->mouseScroll(e, scrollDirection);
		}

		return;
	}

	if (scrollDirection < 0)
		this->_verticalScroll->shift(20);
	else
		this->_verticalScroll->shift(-20);
}

void Kit::Component::setupComponents()
{
	setupChildrenRenderer();

	computeSize();
	computeChildrenSize();

	setupBackgroundImage();
	setupFont();
	setupText();
}


int Kit::Component::width() const
{
	return _innerSize.w();
}

int Kit::Component::height() const
{
	return _innerSize.h();
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

const Kit::vector<Kit::Component*>& Kit::Component::childs() const
{
	return _childrens;
}

Kit::Window* Kit::Component::window() const
{
	return _window;
}

bool Kit::Component::isChildrenObject(Component* obj) const
{
	bool is_child = false;

	is_child = this == obj;


	if (_childrens.size() != 0 && is_child == false)
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
	bool is_parent = false;

	if (_parent == nullptr)
	{
		is_parent = this == obj;
	}
	else
	{
		is_parent |= _parent->isParentObject(obj);
	}

	return is_parent;
}

Kit::Component* Kit::Component::append(Component* component)
{
	if (component != nullptr)
	{
		this->_childrens.push_back(component);
		component->_parent = this;
	
		setupParentWindow();
	}

	return component;
}

Kit::Component* Kit::Component::append(vector<Component*> components)
{
	for (auto& component : components)
	{
		append(component);
	}

	return this;
}

bool Kit::Component::onHover(Point point)
{
	return _isDisplay && point.in(_outerSize);
}

Kit::Component* const Kit::Component::onComponentHover(Point point)
{
	point.dy(_verticalScroll->_nowValue);
	point.dx(_horizontalScroll->_nowValue);

	for (int i = _childrens.size() - 1; i >= 0; i--)
	{
		auto& children = _childrens[i];

		if (children->onHover(point))
		{
			// adjust coord
			point = point - children->outerSize().start;

			return children->onComponentHover(point);
		}
	}

	return this;
}

Kit::string Kit::Component::id() const
{
	return _id;
}

void Kit::Component::addEventListener(string action, eventCallback callback_function)
{
	_eventListeners[action] = callback_function;
}

void Kit::Component::removeEventListener(string action)
{
	_eventListeners[action] = Component::_emptyCallback;
}

void Kit::Component::callEventListener(string action, Event* e)
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

Kit::map<Kit::string, void*>& Kit::Component::userData()
{
	return _userData;
}

void Kit::Component::addUserData(string key, void* data)
{
	_userData.insert(make_pair(key, data));
}

void* Kit::Component::userData(string key)
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

bool Kit::Component::hasClass(string className) const
{
	return _classes.find(className) != -1;
}

Kit::Component* Kit::Component::removeClass(string className)
{
	int indexStart = _classes.find(className);

	if (indexStart == -1)
		return this;

	_classes.erase(_classes.begin() + indexStart, _classes.begin() + indexStart + className.length());


	_window->handleStyles();
	return this;
}

Kit::Component* Kit::Component::addClass(string className)
{
	_classes += " " + className;

	_window->handleStyles();
	return this;
}

Kit::Component* Kit::Component::toggleClass(string className)
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

void Kit::Component::setText(string text)
{
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

void Kit::Component::include(string path)
{
	if (_css_component == nullptr)
		_css_component = new CSS::css(path);

	_css_component->open(path);

}


