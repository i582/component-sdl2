#pragma once

#include "SDL.h"
#include "../../utils/utils.h"


namespace Kit
{
    using std::string;
    using std::cout;
    using std::endl;

    class Rect;

    class Size;

    class Point
    {
    public:
        /**
         * @brief Parse the string and calculates the value in pixels
         * @param str String for parse (must contained px or %)
         * @param parentValue Value relative to which values ​​containing percentages are calculated
         * @return Calculated value or -1 if error
         */
        [[nodiscard]] static int parseStringToNumber(const string& str, int relativeValue = -1);

        /**
         * @brief Parsing an expression of the form (x + y) or (x - y), where x and y can be
         * both percentages (for example 25%) and a value in pixels (for example 200px)
         *
         * @param str String for parse (must contained px or %)
         * @param parentValue Value relative to which values ​​containing percentages are calculated
         * @return Calculated value or -1 if error
         */
        [[nodiscard]] static int parseExpressionToNumber(const string& str, int relativeValue = -1);

        /**
         * @brief Part for numerical size
         */
    private:
        int _x, _y;

    public:
        Point();
        Point(int x, int y);
        Point(const Point& obj);

        explicit Point(const SDL_Point& p);
        explicit Point(SDL_Point* p);

        Point operator+(const Point& obj) const;
        Point operator-(const Point& obj) const;
        Point operator*(const int& scale) const;
        Point operator*(const double& scale) const;
        Point operator/(const int& scale) const;
        Point operator/(const double& scale) const;
        bool operator==(const Point& obj) const;
        bool operator!=(const Point& obj) const;
        Point& operator=(const Point& obj);

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
        Point(const string& top, const string& left);

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
        [[nodiscard]] int x() const;
        [[nodiscard]] int y() const;

        int x(int v);
        int y(int v);

        int dx(int d);
        int dy(int d);

        [[nodiscard]] int* px();
        [[nodiscard]] int* py();

        /**
         * @brief
         * @return Point struct understood by SDL API
         */
        [[nodiscard]] SDL_Point toSDLPoint() const;

        /**
         * @brief Checks if a given point lies in a rectangle
         * @param r Rect or SDL_Rect
         * @param ignorePosition flag meaning not taking into account the position of the rectangle
         * @return [true|false]
         */
        [[nodiscard]] bool in(const Rect&, bool ignorePosition = false) const;
        [[nodiscard]] bool in(const SDL_Rect& r) const;
        [[nodiscard]] bool in(SDL_Rect* r) const;
    };


}






