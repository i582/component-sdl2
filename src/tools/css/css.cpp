#include "css.h"

CSS::css::css()
{
    this->parser = nullptr;
}

CSS::css::css(const string& path)
{
    this->parser = new css_parser(path, this);
    parse();
}

CSS::css::css(const string& code, bool isCode)
{
    this->parser = new css_parser(code, true, this);
    parse();
}

CSS::css::~css()
{
    delete parser;
}

void CSS::css::open(const string& path)
{
    delete parser;
    this->parser = new css_parser(path, this);
    parse();
}

void CSS::css::parse()
{
    if (this->parser != nullptr)
    {
        parser->parse();
    }
    else
    {
        cout << "ERROR: parser not configured. Set the code or path to the CSS lib!" << endl;
    }
}

void CSS::css::add(const css_block& block)
{
    blocks[block.name()] = block;
}

CSS::css_block& CSS::css::operator[](const string& key)
{
    return at(key);
}

CSS::css_block& CSS::css::at(const string& key)
{
    if (blocks.find(key) == blocks.end())
    {
        cout << "ERROR: Block with id/classname" << key << " NOT found!" << endl;
    }

    return blocks[key];
}

void CSS::css::set_code(const string& code)
{
    delete parser;
    this->parser = new css_parser(code, true, this);
}

CSS::map<CSS::string, CSS::css_block>& CSS::css::getStyles()
{
    return blocks;
}
