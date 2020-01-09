#include "css.h"

CSS::css::css()
{
    this->_parser = nullptr;
}

CSS::css::css(const string& path)
{
    this->_parser = new css_parser(path, this);
    parse();
}

CSS::css::css(const string& code, bool isCode)
{
    this->_parser = new css_parser(code, true, this);
    parse();
}

CSS::css::~css()
{
    delete _parser;
}

void CSS::css::open(const string& path)
{
    delete _parser;
    this->_parser = new css_parser(path, this);
    parse();
}

void CSS::css::parse()
{
    if (this->_parser != nullptr)
    {
        _parser->parse();
    }
    else
    {
        cout << "ERROR: parser not configured. Set the code or path to the CSS lib!" << endl;
    }
}

void CSS::css::add(const css_block& block)
{
    _blocks[block.name()] = block;
}

CSS::css_block& CSS::css::operator[](const string& key)
{
    return at(key);
}

CSS::css_block& CSS::css::at(const string& key)
{
    if (_blocks.find(key) == _blocks.end())
    {
        cout << "ERROR: Block with id/classname" << key << " NOT found!" << endl;
    }

    return _blocks[key];
}

void CSS::css::setCode(const string& code)
{
    delete _parser;
    this->_parser = new css_parser(code, true, this);
}

CSS::map<CSS::string, CSS::css_block>& CSS::css::getStyles()
{
    return _blocks;
}
