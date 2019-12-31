#pragma once

#include "css_block_state.h"

namespace CSS
{

class css_block
{
private:
	css_block_state _normal;
	css_block_state _hover;
	css_block_state _active;
    css_block_state _focus;

	string _name;

public:

	/*
	 * @brief Simple constructor, name equal "" 
	 */
	css_block();


	/*
	 * @brief Constructor with block name
	 * @param [in] name: Block name
	 */
	css_block(const string& name);


	/*
	 * @brief Constructor with block name that sets the default styles
	 *
	 * @param [in] name: Block name
	 * @param [in] isCode: Flag to setting default styles
	 */
	css_block(const string& name, bool default_style);

public:

	/*
	 * @brief Function for merge two blocks
	 * @param [in] block: Reference to block to merge
	 */
	void mergeWith(css_block& block);


	/*
	 * @brief Function for get the normal block state
	 * @return [css_block_state&] Block state reference
	 */
	css_block_state& normal();
	/*
	 * @brief Function for get the hover state
	 * @return [css_block_state&] Block state reference
	 */
	css_block_state& hover();
	/*
	 * @brief Function for get the active state
	 * @return [css_block_state&] Block state reference
	 */
	css_block_state& active();
    /*
     * @brief Function for get the focus state
     * @return [css_block_state&] Block state reference
     */
    css_block_state& focus();


	/*
	 * @brief Functions for setup normal state block
	 * @param [in] name: State block
	 */
	void normal(const css_block_state& new_block);
	/*
	 * @brief Functions for setup hover state block
	 * @param [in] name: State block
	 */
	void hover(const css_block_state& new_block);
	/*
	 * @brief Functions for setup active state block
	 * @param [in] name: State block
	 */
	void active(const css_block_state& new_block);
    /*
	 * @brief Functions for setup active state block
	 * @param [in] name: State block
	 */
    void focus(const css_block_state& new_block);


	/*
	 * @brief Function for get the block name
	 * @return [string] Block name
	 */
	string name() const;


	/*
	 * @brief Function for setup the block name
	 * @param [in] name: Block name
	 */
	void name(const string& name);
};


}