#pragma once

#include "SDL.h"
#include "../../point/extended-point/extended-point.h"


namespace Kit
{

class Rect;

class Size
{
	/**
	 * @brief Part for numerical size
	 */
private:
	int _w, _h;

public:
	Size();
	Size(int w, int h);
	Size(const Size& obj);


	Size operator+(const Size& obj);
	Size operator-(const Size& obj);
	Size operator*(const int& scale);
	Size operator*(const double& scale);
	Size operator/(const int& scale);
	Size operator/(const double& scale);

	bool operator==(const Size& obj) const;
	bool operator!=(const Size& obj) const;
	
	Size& operator=(const Size& obj);

	/**
	 * @brief Part for where definition from a string, like 100px or 24%
	 */
public:
	/**
	 * @brief Strings must contain px or %
	 */
	string width, height;

	bool isCalculated;

public:
	Size(const string& width, const string& height);

	/**
	 * @brief Function calculating sizes from strings
	 * @param parent Size relative to which values ​​containing percentages are calculated
	 * @return Rectangle with calculated values
	 */
	Size calc(const Rect& parent);


	/**
	 * @brief Our friends
	 */
public:
	friend Rect;
	friend Point;

	/**
	 * @brief general interface
	 */
public:
	int w() const;
	int h() const;
	int w(int v);
	int h(int v);

	int dw(int d);
	int dh(int d);
};


}