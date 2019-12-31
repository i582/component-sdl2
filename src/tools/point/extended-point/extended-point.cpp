#include "extended-point.h"
#include "../../rect/extended-rect/extended-rect.h"

using namespace Kit;

int Point::parseStringToNumber(const string& str, int relativeValue)
{
	int index = 0;

	if ((index = str.find("px")) != -1)
	{
		return stoi(str.substr(0, index));
	}
	else if ((index = str.find('%')) != -1)
	{
		if (relativeValue == -1)
		{
			cout << "ERROR: relative value not set!" << endl;
			return -1;
		}

		return (int)((stoi(str.substr(0, index)) / 100.) * relativeValue);
	}
	else if (str.find("auto") != -1)
	{
		return 0xfffffff;
	}
	else if (str[0] == '0')
    {
        return 0;
    }
	else
	{
		cout << "ERROR: string does not contain 'px' or '%'!" << endl;
		return -1;
	}
}

int Point::parseExpressionToNumber(const string& str, int relativeValue)
{
	int result = -1;

	int findPlus = -1, findMinus = -1;
	vector <string>* expression = nullptr;


	if ((findMinus = str.find("-")) != -1)
	{
		expression = Utils::split(str, '-');
	}
	else if ((findPlus = str.find("+")) != -1)
	{
		expression = Utils::split(str, '+');
	}
	else
	{
		return parseStringToNumber(str, relativeValue);
	}


	if (expression->size() != 2)
	{
		std::cout << "ERROR: Wrong expression (" << str << ")" << std::endl;
		return -1;
	}

	int first = parseStringToNumber(expression->at(0), relativeValue);
	int second = parseStringToNumber(expression->at(1), relativeValue);

	if (findPlus != -1)
	{
		result = first + second;
	}
	else if (findMinus != -1)
	{
		result = first - second;
	}

	delete expression;

	return result;
}

Point::Point() : Point(0, 0) {}

Point::Point(int x, int y)
{
	this->_x = x;
	this->_y = y;
	this->isCalculated = true;
}

Point::Point(const Point& obj)
{
	this->_x = obj._x;
	this->_y = obj._y;
	this->top = obj.top;
	this->left = obj.left;
	this->isCalculated = obj.isCalculated;
}


Point::Point(const SDL_Point& p) : Point(p.x, p.y) {}
Point::Point(SDL_Point* p) : Point(p->x, p->y) {}


Point::Point(const string& left, const string& top)
{
	this->left = left;
	this->top = top;
	this->isCalculated = false;
}

Point Point::operator+(const Point& obj)
{
	return { this->_x + obj._x, this->_y + obj._y };
}

Point Point::operator-(const Point& obj)
{
	return { this->_x - obj._x, this->_y - obj._y };
}

bool Point::operator==(const Point& obj) const
{
	return this->_x == obj._x && this->_y == obj._y;
}

bool Point::operator!=(const Point& obj) const
{
	return this->_x != obj._x || this->_y != obj._y;
}

Point& Kit::Point::operator=(const Point& obj)
{
	this->_x = obj._x;
	this->_y = obj._y;

	this->top = obj.top;
	this->left = obj.left;

	this->isCalculated = obj.isCalculated;

	return *this;
}

Point Point::operator*(const int& scale)
{
	return { this->_x * scale, this->_y * scale };
}

Point Point::operator*(const double& scale)
{
	return { (int)(this->_x * scale), (int)(this->_y * scale) };
}

Point Point::operator/(const int& scale)
{
	return { this->_x / scale, this->_y / scale };
}

Point Point::operator/(const double& scale)
{
	return { (int)(this->_x / scale), (int)(this->_y / scale) };
}


Point Point::calc(const Rect& parent)
{
	/*if (isCalculated)
		return *this;*/

	this->_x = Point::parseExpressionToNumber(left, parent.size._w);
	this->_y = Point::parseExpressionToNumber(top, parent.size._h);

	this->isCalculated = true;

	return *this;
}

int Point::x() const
{
	if (!isCalculated)
	{
		cout << "Error: String were not parsed into values. See Point's 'calc' function." << endl;
		return -1;
	}

	return _x;
}

int Point::y() const
{
	if (!isCalculated)
	{
		cout << "Error: String were not parsed into values. See Point's 'calc' function." << endl;
		return -1;
	}

	return _y;
}

int Point::x(int v)
{
	_x = v;
	return _x;
}

int Point::y(int v)
{
	_y = v;
	return _y;
}

int Point::dx(int d)
{
	_x += d;
	return _x;
}

int Point::dy(int d)
{
	_y += d;
	return _y;
}

int* Point::px()
{
	return &_x;
}

int* Point::py()
{
	return &_y;
}

SDL_Point Point::toSDLPoint() const
{
	return { _x, _y };
}

bool Point::in(const Rect& rect, bool ignorePosition) const
{
	SDL_Point p = { _x, _y };
	SDL_Rect r = rect.toSdlRect();

	if (ignorePosition)
	{
		r.x = 0;
		r.y = 0;
	}

	return SDL_PointInRect(&p, &r);
}

bool Point::in(SDL_Rect* r) const
{
	SDL_Point p = { _x, _y };
	return SDL_PointInRect(&p, r);
}

bool Point::in(const SDL_Rect& r) const
{
    SDL_Point p = { _x, _y };
    return SDL_PointInRect(&p, &r);
}
