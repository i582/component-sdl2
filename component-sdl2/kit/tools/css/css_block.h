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
	css_block(string name);


	/*
	 * @brief Constructor with block name that sets the default styles
	 *
	 * @param [in] name: Block name
	 * @param [in] isCode: Flag to setting default styles
	 */
	css_block(string name, bool general);

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
	 * @brief Function for get the normal hover state
	 * @return [css_block_state&] Block state reference
	 */
	css_block_state& hover();
	/*
	 * @brief Function for get the normal active state
	 * @return [css_block_state&] Block state reference
	 */
	css_block_state& active();


	/*
	 * @brief Functions for settup normal state block
	 * @param [in] name: State block
	 */
	void normal(css_block_state new_block);
	/*
	 * @brief Functions for settup hover state block
	 * @param [in] name: State block
	 */
	void hover(css_block_state new_block);
	/*
	 * @brief Functions for settup active state block
	 * @param [in] name: State block
	 */
	void active(css_block_state new_block);


	/*
	 * @brief Function for get the block name
	 * @return [string] Block name
	 */
	string name();


	/*
	 * @brief Function for settup the block name
	 * @param [in] name: Block name
	 */
	void name(string name);
};


}