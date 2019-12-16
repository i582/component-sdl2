#include "component.h"
#include "../window/window.h"

Lib::Component* Lib::Component::_hoverComponent = nullptr;

Lib::Component::Component(string id, Rect size, string classes) : Component(id, size, classes, {}) {}

Lib::Component::Component(string id, Rect size, string classes, vector<Component*> childrens)
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


	/** Text */
	Font::root("../component-sdl2/fonts/");
	this->_font = new Font("segoeui");
	this->_text = new Text(this, "", { 0,0,0,0 }, _font, 14, Color("#000000"));


	/** Scroll */
	this->_scroll = new Scroll(_renderer, { 0, 0, 15, 0 }, 0, 1);
	this->_scrollable = false;
	this->_isScrollActive = false;

	/** Image */
	this->_image = nullptr;

	/** CSS component */
	CSS::css* _css_component = nullptr;
}

Lib::Component::~Component()
{
	SDL_DestroyTexture(_innerTexture);
	SDL_DestroyTexture(_outerTexture);

	delete _scroll;
	delete _font;
	delete _text;
	delete _image;
	delete _css_component;
}

void Lib::Component::setupEventListeners()
{
	_eventListeners["click"] = Component::_emptyCallback;
	_eventListeners["hover"] = Component::_emptyCallback;
	_eventListeners["mousemotion"] = Component::_emptyCallback;
	_eventListeners["onmousedown"] = Component::_emptyCallback;
	_eventListeners["onmouseup"] = Component::_emptyCallback;
	_eventListeners["onmouseover"] = Component::_emptyCallback;
	_eventListeners["onmouseout"] = Component::_emptyCallback;
}


Lib::Component* Lib::Component::getFirstScrollableParent()
{
	if (this->_parent != nullptr)
	{
		if (this->_parent->isScrollable())
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

CSS::css* Lib::Component::getComponentStyles()
{
	return this->_css_component;
}

void Lib::Component::setupChildrenRenderer()
{
	if (this->_renderer == nullptr && this->_parent != nullptr)
	{
		this->_renderer = _parent->renderer();

		this->_text->setRenderer(this->_renderer);
	}

	for (auto& children : _childrens)
	{
		children->setupChildrenRenderer();
	}
}

void Lib::Component::computeSize()
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


	/** Устанавливаем размер текста */
	this->_text->setSize({ 0, 0, _innerSize.w(), _innerSize.h() });


	for (auto& children : _childrens)
	{
		children->computeSize();
	}
}

void Lib::Component::computeChildrenSize()
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
	}

	if (childrenSize.h > _innerSize.h())
	{
		newTextureSize.h(childrenSize.h);



		/*
		 * Setup scroll TO DO
		 */
		this->_scroll->_bodySize.x(_innerSize.x() + _innerSize.w());
		this->_scroll->_bodySize.y(_innerSize.y());

		this->_scroll->_bodySize.h(_innerSize.h());

		this->_scroll->_maxValue = newTextureSize.h() - _innerSize.h();

		this->_scroll->_relSizes = _innerSize.h() / (double)newTextureSize.h();
		this->_scroll->_renderer = _renderer;
		this->_scroll->init();

		_outerSize.dw(this->_scroll->_bodySize.w());

		SDL_DestroyTexture(this->_outerTexture);
		this->_outerTexture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _outerSize.w(), _outerSize.h());
		SDL_SetTextureBlendMode(_outerTexture, SDL_BLENDMODE_BLEND);

		this->_scroll->_parentTexture = this->_outerTexture;
		this->_scrollable = true;
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

void Lib::Component::setupBackgroundImage()
{
	this->_image = new Image(this);

	for (auto& children : _childrens)
	{
		children->setupBackgroundImage();
	}
}

void Lib::Component::setupParentWindow()
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

void Lib::Component::adjustMousePoint(Point& p)
{
	if (_parent == nullptr)
		return;

	p = p - _outerSize.start;

	_parent->adjustMousePoint(p);
}

void Lib::Component::render()
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


	if (_isHovered)
	{
		if (_isActive)
		{
			blockState = &_cssBlock.active();
		}
		else
		{
			blockState = &_cssBlock.hover();
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
	copy.x(0); copy.y(this->_scroll->_nowValue);

	SDL_RenderCopy(_renderer, _innerTexture, &copy.toSdlRect(), &_innerSize.toSdlRect());


	_scroll->render();


	SDL_Texture* parentTexture = nullptr;
	if (_parent != nullptr)
	{
		parentTexture = _parent->innerTexture();
	}

	SDL_SetRenderTarget(_renderer, parentTexture);
	SDL_RenderCopy(_renderer, _outerTexture, NULL, &_outerSize.toSdlRect());
}

void Lib::Component::mouseButtonDown(Event* e)
{
	if (!_isDisplay)
		return;


	Point mouseP(e->motion.x, e->motion.y);

	adjustMousePoint(mouseP);


	if (_scroll->onHoverSlider(mouseP))
	{
		cout << "Scroll slider" << endl;

		cout << "scroll active TRUE" << endl;
		_isScrollActive = true;

		return;
	}


	_eventListeners["click"](this, e);
	_eventListeners["onmousedown"](this, e);

	_isActive = true;
}

void Lib::Component::mouseButtonUp(Event* e)
{
	if (!_isDisplay)
		return;

	_eventListeners["onmouseup"](this, e);

	//cout << "scroll active FALSE" << endl;
	_isScrollActive = false;
	_isActive = false;
}

void Lib::Component::mouseMotion(Event* e)
{
	if (!_isDisplay)
		return;

	if (_isScrollActive)
	{
		_scroll->shift(e->motion.yrel * (_childrensSize.h() / _innerSize.h()));
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

void Lib::Component::mouseOut(Event* e)
{
	if (!_isDisplay)
		return;

	_eventListeners["onmouseout"](this, e);

	_isHovered = false;
	_isEnterInComponent = false;
}

void Lib::Component::mouseScroll(Event* e, int scrollDirection)
{
	if (!_isDisplay)
		return;

	if (!_scrollable)
	{
		Component* firstScrollableParent = getFirstScrollableParent();

		if (firstScrollableParent != nullptr)
		{
			firstScrollableParent->mouseScroll(e, scrollDirection);
		}

		return;
	}

	if (scrollDirection < 0)
		_scroll->shift(20);
	else
		_scroll->shift(-20);
}

void Lib::Component::setupComponents()
{
	setupChildrenRenderer();

	computeSize();
	computeChildrenSize();

	setupBackgroundImage();
}


int Lib::Component::width() const
{
	return _innerSize.w();
}

int Lib::Component::height() const
{
	return _innerSize.h();
}

int Lib::Component::top() const
{
	return _outerSize.y();
}

int Lib::Component::left() const
{
	return _outerSize.x();
}

const Lib::Rect& Lib::Component::size() const
{
	return _innerSize;
}

const Lib::Rect& Lib::Component::outerSize() const
{
	return _outerSize;
}

const Lib::Rect& Lib::Component::innerSize() const
{
	return _innerSize;
}

Lib::Component* Lib::Component::show()
{
	_isDisplay = true;
	return this;
}

Lib::Component* Lib::Component::hide()
{
	_isDisplay = false;
	return this;
}

Lib::Component* Lib::Component::toggleDisplay()
{
	_isDisplay = !_isDisplay;
	return this;
}

bool Lib::Component::display() const
{
	return _isDisplay;
}

SDL_Renderer* Lib::Component::renderer() const
{
	return _renderer;
}

SDL_Texture* Lib::Component::innerTexture() const
{
	return _innerTexture;
}

SDL_Texture* Lib::Component::outerTexture() const
{
	return _outerTexture;
}

Lib::Component* Lib::Component::parent() const
{
	return _parent;
}

const Lib::vector<Lib::Component*>& Lib::Component::childs() const
{
	return _childrens;
}

Lib::Window* Lib::Component::window() const
{
	return _window;
}

bool Lib::Component::isChildrenObject(Component* obj) const
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

bool Lib::Component::isParentObject(Component* obj) const
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

Lib::Component* Lib::Component::append(Component* component)
{
	if (component != nullptr)
	{
		this->_childrens.push_back(component);
		component->_parent = this;
	
		setupParentWindow();
	}

	return component;
}

Lib::Component* Lib::Component::append(vector<Component*> components)
{
	for (auto& component : components)
	{
		append(component);
	}

	return this;
}

bool Lib::Component::onHover(Point point)
{
	return _isDisplay && point.in(_outerSize);
}

Lib::Component* const Lib::Component::onComponentHover(Point point)
{
	point.dy(_scroll->_nowValue);

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

Lib::string Lib::Component::id() const
{
	return _id;
}

void Lib::Component::addEventListener(string action, eventCallback callback_function)
{
	_eventListeners[action] = callback_function;
}

void Lib::Component::removeEventListener(string action)
{
	_eventListeners[action] = Component::_emptyCallback;
}

void Lib::Component::callEventListener(string action, Event* e)
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

Lib::map<Lib::string, void*>& Lib::Component::userData()
{
	return _userData;
}

void Lib::Component::addUserData(string key, void* data)
{
	_userData.insert(make_pair(key, data));
}

void* Lib::Component::userData(string key)
{
	return _userData[key];
}

bool Lib::Component::isHovered()
{
	return _isHovered;
}

bool Lib::Component::isActive()
{
	return _isActive;
}

bool Lib::Component::hasClass(string className) const
{
	return _classes.find(className) != -1;
}

Lib::Component* Lib::Component::removeClass(string className)
{
	int indexStart = _classes.find(className);

	if (indexStart == -1)
		return this;

	_classes.erase(_classes.begin() + indexStart, _classes.begin() + indexStart + className.length());


	_window->handleStyles();
	return this;
}

Lib::Component* Lib::Component::addClass(string className)
{
	_classes += " " + className;

	_window->handleStyles();
	return this;
}

Lib::Component* Lib::Component::toggleClass(string className)
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

void Lib::Component::setText(string text)
{
	_text->setText(text);
}

bool Lib::Component::isScrollable() const
{
	return _scrollable;
}

void Lib::Component::include(string path)
{
	if (_css_component == nullptr)
		_css_component = new CSS::css(path);

	_css_component->open(path);

}


