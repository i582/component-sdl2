#include "component.h"

Lib::Component::Component(string id, Rect size, string classes)
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
	this->_childrens;
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


	/** Styles */ // TO DO
	// this->_cssBlock = CSS::css_block(id, true);


	/** Other for Events */
	this->_isEnterInComponent = false;


	/** Image */
	this->_image = nullptr;
}

Lib::Component::~Component()
{
	SDL_DestroyTexture(_innerTexture);
	SDL_DestroyTexture(_outerTexture);

	//delete _scroll;
	//delete _text;
	delete _image;
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
	return nullptr;
}

void Lib::Component::setupChildrenRenderer()
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
		/*this->scroll->_bodySize.x(_innerSize.x() + _innerSize.w());
		this->scroll->_bodySize.y(_innerSize.y());

		this->scroll->_bodySize.h(_innerSize.h());

		this->scroll->_maxValue = newTextureSize.h() - _innerSize.h();

		this->scroll->_relSizes = _innerSize.h() / (double)newTextureSize.h();
		this->scroll->_renderer = _renderer;
		this->scroll->init();

		_outerSize.dw(this->scroll->_bodySize.w());

		SDL_DestroyTexture(this->_outerTexture);
		this->_outerTexture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _outerSize.w(), _outerSize.h());
		SDL_SetTextureBlendMode(_outerTexture, SDL_BLENDMODE_BLEND);

		this->scroll->_parentTexture = this->_outerTexture;
		this->scrollable = true;*/
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

void Lib::Component::setupContainer()
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

bool Lib::Component::onHover(Point point)
{
	return _isDisplay && point.in(_outerSize);
}

Lib::Component* const Lib::Component::onContainerHover(Point point)
{
	// point.dy(scroll->_nowValue);

	for (int i = _childrens.size() - 1; i >= 0; i--)
	{
		auto& children = _childrens[i];

		if (children->onHover(point))
		{
			// adjust coord
			point = point - children->outerSize().start;

			return children->onContainerHover(point);
		}
	}

	return this;
}

Lib::string& Lib::Component::id()
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


	//_window->handleStyles();
	return this;
}

Lib::Component* Lib::Component::addClass(string className)
{
	_classes += " " + className;

	//_window->handleStyles();
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


