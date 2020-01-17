/*
 * A small library for parsing a small subset of the CSS language. 
 * This list includes blocks and pseudo-classes (hover, active, focus)
 * 2020 (c) MIT
 */


#pragma once

#include <string>

#include "css_block.h"
#include "css_parser.h"
#include "../utils/utils.h"

namespace CSS
{

    using std::string;
    using std::map;

    class css
    {
    private:
        map<string, css_block> _blocks;
        css_parser* _parser;

    public:
        /*
         * @brief Simple constructor, parser not configured
         */
        css();


        /*
         * @brief Constructor with style lib path, parser starts automatically
         * if lib not found, an exception will be thrown
         *
         * @param [in] Path to lib
         */
        explicit css(const string& path);


        /*
         * @brief Constructor with css-code, parser starts automatically
         *
         * @param [in] code: String with css-code
         * @param [in] isCode: Flag to separate code from lib
         */
        css(const string& code, bool isCode);


        /*
         * @brief Sad destructor without description :(
         */
        ~css();

    public:

        /*
         * @brief Function for open css style lib, parser starts automatically
         * if lib not found, an exception will be thrown
         *
         * If the parser created with a simple constructor, then this function is required
         *
         * @param [in] Path to lib
         */
        void open(const string& path);


        /*
         * @brief Function for start parse, if the parser is configured, if not
         * print error message and does nothing
         */
        void parse();


        /*
         * @brief Function for add a new css block
         * @param [in] block: New block to add
         */
        void add(const css_block& block);


        /*
         * @brief Function to get reference to css block by name
         * @param [in] key: Block name
         *
         * @return [css_block&] Reference to css_block
         */
        css_block& at(const string& key);

        /*
         * @brief Same as the "at" function
         */
        css_block& operator[](const string& key);


        /*
         * @brief Function to setting the code, if parser was configured using
         * a simple constructor
         * @param [in] code: Code string
         */
        void setCode(const string& code);


        /*
         * @brief Function to get reference to style
         * @return [map<string, css_block>&] Reference to map with styles
         */
        map<string, css_block>& getStyles();
    };

}
