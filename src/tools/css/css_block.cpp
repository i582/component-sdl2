#include "css_block.h"

CSS::css_block::css_block() : css_block("") {}

CSS::css_block::css_block(const string& name)
{
	this->_name = name;

	this->_normal = css_block_state();
	this->_hover = css_block_state();
	this->_active = css_block_state();
    this->_focus = css_block_state();
}

CSS::css_block::css_block(const string& name, bool default_style)
{
	this->_name = name;

	this->_normal = css_block_state(default_style);
	this->_hover = css_block_state(default_style);
	this->_active = css_block_state(default_style);
    this->_focus = css_block_state(default_style);
}

void CSS::css_block::mergeWith(css_block& block)
{
	_normal.mergeWith(block._normal);
	_hover.mergeWith(block._hover);
	_active.mergeWith(block._active);
    _focus.mergeWith(block._focus);
}

CSS::css_block_state& CSS::css_block::normal()
{
	return _normal;
}

CSS::css_block_state& CSS::css_block::hover()
{
	return _hover;
}

CSS::css_block_state& CSS::css_block::active()
{
	return _active;
}

CSS::css_block_state& CSS::css_block::focus()
{
    return _focus;
}

void CSS::css_block::normal(const css_block_state& new_block)
{
	this->_normal = new_block;
}

void CSS::css_block::hover(const css_block_state& new_block)
{
	this->_hover = new_block;
}

void CSS::css_block::active(const css_block_state& new_block)
{
	this->_active = new_block;
}

void CSS::css_block::focus(const CSS::css_block_state& new_block)
{
    this->_focus = new_block;
}

CSS::string CSS::css_block::name() const
{
	return _name;
}

void CSS::css_block::name(const string& name)
{
	this->_name = name;
}

