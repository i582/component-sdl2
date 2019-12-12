#pragma once

#include "SDL.h"
#include "../../utils/utils.h"

namespace Lib
{

using std::string;
using std::cout;
using std::endl;

class Rect;

class Point
{
public:
	/**
	 * @brief Parse the string and calculates the value in pixels
	 * @param str String for parse (must contained px or %)
	 * @param parentValue Value relative to which values ​​containing percentages are calculated
	 * @return Calculated value or -1 if error
	 */
	static int parseStringToNumber(const string& str, int relativeValue = -1);

	/**
	 * @brief Parsing an expression of the form (x + y) or (x - y), where x and y can be 
	 * both percentages (for example 25%) and a value in pixels (for example 200px)
	 * 
	 * @param str String for parse (must contained px or %)
	 * @param parentValue Value relative to which values ​​containing percentages are calculated
	 * @return Calculated value or -1 if error
	 */
	static int parseExpressionToNumber(const string& str, int relativeValue = -1);

	/**
	 * @brief Part for numerical size
	 */
private:
	int _x, _y;

public:
	Point();
	Point(int x, int y);
	Point(const Point& obj);
	
	explicit Point(SDL_Point p);
	explicit Point(SDL_Point& p);
	explicit Point(SDL_Point* p);

	Point operator+(const Point& obj);
	Point operator-(const Point& obj);
	Point operator*(int scale);
	Point operator*(double scale);
	Point operator/(int scale);
	Point operator/(double scale);

	bool operator==(const Point& obj) const;
	bool operator!=(const Point& obj) const;


	/**
	 * @brief Part for where definition from a string, like 100px or 24%
	 */
public:
	/**
	 * @brief Strings must contain px or %
	 */
	string top, left;

	bool isCalculated;

public:
	Point(string top, string left);

	/**
	 * @brief Function calculating sizes from strings
	 * @param parent Size relative to which values ​​containing percentages are calculated
	 * @return Point with calculated values
	 */
	Point calc(const Rect& parent);


public:
	friend Rect;
	friend Size;

	/**
	 * @brief general interface
	 */
public:
	int x() const;
	int y() const;
	int x(int v);
	int y(int v);

	int dx(int d);
	int dy(int d);

	int* px();
	int* py();

	/**
	 * @brief
	 * @return Point struct understood by SDL API
	 */
	SDL_Point toSDLPoint() const;

	/**
	 * @brief Checks if a given point lies in a rectangle
	 * @param r Rect or SDL_Rect
	 * @param ignorePosition flag meaning not taking into account the position of the rectangle
	 * @return [true|false]
	 */
	bool in(Rect, bool ignorePosition = false) const;
	bool in(Rect* r, bool ignorePosition = false) const;

	bool in(SDL_Rect r) const;
	bool in(SDL_Rect* r) const;
};


}






