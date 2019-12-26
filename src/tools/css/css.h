/*
 * A small library for parsing a small subset of the CSS language. 
 * This list includes blocks and pseudo-classes (hover, active)
 * 2019 (c) MIT
 */


#pragma once

#include "css_block.h"
#include "css_parse.h"

#include "string"

#include "../utils/utils.h"

namespace CSS
{

using std::string;
using std::map;

class css
{
private:
	map<string, css_block> blocks;
	css_parser* parser;

public:
	/*
	 * @brief Simple constructor, parser not configured
	 */
	css();


	/*
	 * @brief Constructor with style file path, parser starts automatically
	 * if file not found, an exception will be thrown
	 * 
	 * @param [in] Path to file
	 */
	explicit css(const string& path);


	/*
	 * @brief Constructor with css-code, parser starts automatically
	 *
	 * @param [in] code: String with css-code
	 * @param [in] isCode: Flag to separate code from file 
	 */
	css(const string& code, bool isCode);


	/*
	 * @brief Sad destructor without description :(
	 */
	~css();
	
public:

	/*
	 * @brief Function for open css style file, parser starts automatically
	 * if file not found, an exeception will be thrown
	 *
	 * If the parser created with a simple constructor, then this function is required
	 *
	 * @param [in] Path to file
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
	void set_code(const string& code);


	/*
	 * @brief Function to get reference to style
	 * @return [map<string, css_block>&] Reference to map with styles
	 */
	map<string, css_block>& getStyles();
};

}

// ([^:])string([^"|])
// $1std::string$2