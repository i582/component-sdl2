#include "extended-size.h"
#include "../../rect/extended-rect/extended-rect.h"

using namespace Kit;

Size::Size() : Size(0, 0) {}

Size::Size(int w, int h)
{
	this->_w = w;
	this->_h = h;
	this->isCalculated = true;
}

Size::Size(const Size& obj)
{
	this->_w = obj._w;
	this->_h = obj._h;
	this->width = obj.width;
	this->height = obj.height;
	this->isCalculated = obj.isCalculated;
}

Size Size::operator+(const Size& obj)
{
	Size size;
	size._w = this->_w + obj._w;
	size._h = this->_h + obj._h;
	return size;
}

Size Size::operator-(const Size& obj)
{
	Size size;
	size._w = this->_w - obj._w;
	size._h = this->_h - obj._h;
	return size;
}

bool Size::operator==(const Size& obj) const
{
	return this->_w == obj._w && this->_h == obj._h;
}

bool Size::operator!=(const Size& obj) const
{
	return this->_w != obj._w || this->_h != obj._h;
}

Size Size::operator*(int scale)
{
	Size size;
	size._w = this->_w * scale;
	size._h = this->_h * scale;
	return size;
}

Size Size::operator*(double scale)
{
	Size size;
	size._w = (int)(this->_w * scale);
	size._h = (int)(this->_h * scale);
	return size;
}

Size Size::operator/(int scale)
{
	Size size;
	size._w = this->_w / scale;
	size._h = this->_h / scale;
	return size;
}

Size Size::operator/(double scale)
{
	Size size;
	size._w = (int)(this->_w / scale);
	size._h = (int)(this->_h / scale);
	return size;
}



Size::Size(string width, string height)
{
	this->width = width;
	this->height = height;
	this->isCalculated = false;
}

Size Size::calc(const Rect& parent)
{
	/*if (isCalculated)
		return *this;*/

	this->_w = Point::parseExpressionToNumber(width, parent.size._w);
	this->_h = Point::parseExpressionToNumber(height, parent.size._h);

	this->isCalculated = true;

	return *this;
}

int Size::w() const
{
	if (!isCalculated)
	{
		cout << "Error: String were not parsed into values. See Size's 'calc' function." << endl;
		return -1;
	}

	return _w;
}

int Size::h() const
{
	if (!isCalculated)
	{
		cout << "Error: String were not parsed into values. See Size's 'calc' function." << endl;
		return -1;
	}

	return _h;
}

int Size::w(int v)
{
	_w = v;
	return _w;
}

int Size::h(int v)
{
	_h = v;
	return _h;
}

int Size::dw(int d)
{
	_w += d;
	return _w;
}

int Size::dh(int d)
{
	_h += d;
	return _h;
}
