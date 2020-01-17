#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "SDL.h"

namespace Kit
{

    using std::string;
    using std::vector;
    using std::cout;
    using std::endl;

    namespace Utils
    {
        /**
         * @brief Function to convert a string to an integer
         * @param str String for convert
         */
        int to_integer(const string& str);

        /**
         * @brief Function to convert a string to a double
         * @param str String for convert
         */
        double to_double(const string& str);


        /**
         * @brief Split the string by into substring by separator (symbol)
         * @param str String for split
         * @param symbol The symbol to split the string into
         * @return Array of string
         */
        vector<string>* split(const string& str, char symbol);


        /**
         * @brief Splits a string into a substring by any of the delimiters in the pattern string
         * @param str String for split
         * @param pattern String containing a set of separators
         * @param separator Symbol for splitting a pattern string into separate delimiters
         * @return Array of string
         */
        vector<string>* split(const string& str, const string& pattern, char separator = '\t');


        /**
         * @brief Returns a lowercase string
         */
        string to_lower(const string& str);


        /**
         * @brief Returns a uppercase string
         */
        string to_upper(const string& str);


        /**
         * @brief Check string for integer
         * @param str String for check
         * @return true|false
         */
        bool is_integer(const string& str);

        /**
         * @brief Check string for number
         * @param str String for check
         * @return true|false
         */
        bool is_number(const string& str);


        template<typename T, typename... Args>
        T max_of(T a, Args... args)
        {
            if constexpr (sizeof...(args) == 0)
            {
                return a;
            }
            else if constexpr (sizeof...(args) == 1)
            {
                return (a > max_of(args...)) ? (a) : (max_of(args...));
            }
            else
            {
                return max_of(a, max_of(args...));
            }
        }

        template<typename T, typename... Args>
        T min_of(T a, Args... args)
        {
            if constexpr (sizeof...(args) == 0)
            {
                return a;
            }
            else if constexpr (sizeof...(args) == 1)
            {
                return (a < min_of(args...)) ? (a) : (min_of(args...));
            }
            else
            {
                return min_of(a, min_of(args...));
            }
        }
    }


}