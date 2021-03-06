#pragma once

#include "SDL.h"

#include "iostream"

#include "string"
#include "vector"

namespace CSS
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
        vector<string>* split(string str, char symbol);


        /**
         * @brief Splits a string into a substring by any of the delimiters in the pattern string
         * @param str String for split
         * @param pattern String containing a set of separators
         * @param separator Symbol for splitting a pattern string into separate delimiters
         * @return Array of string
         */
        vector<string>* split(string str, string pattern, char separator = '\t');


        /**
         * @brief Check string for integer
         * @param str String for check
         * @return true|false
         */
        bool is_integer(string str);

        /**
         * @brief Check string for number
         * @param str String for check
         * @return true|false
         */
        bool is_number(string str);


    }


}