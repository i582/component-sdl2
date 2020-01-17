#include "css_parser.h"
#include "css.h"


CSS::css_parser::css_parser(const string& file_path_, CSS::css* css_parent_)
{
    if (css_parent_ == nullptr)
    {
        cout << "ERROR: css_parent is nullptr" << endl;
        return;
    }


    this->css_parent = css_parent_;
    this->file_path = file_path_;

    openFile();
}

CSS::css_parser::css_parser(const string& code_, bool is_code, CSS::css* css_parent_)
{
    if (css_parent_ == nullptr)
    {
        cout << "ERROR: css_parent is nullptr" << endl;
        return;
    }


    this->css_parent = css_parent_;
    this->code = code_;

}

void CSS::css_parser::parse()
{
    splitByToken();
    splitByBlock();
    syntaxParse();
    mergeStyleComponent();
    updateCSS();
}


void CSS::css_parser::openFile()
{
    std::ifstream in(file_path, std::ios::binary);
    int size = in.seekg( 0, std::ios::end).tellg();
    in.seekg(0);


    this->code.resize(size + 1);

    in.read(&code[0], size);

    code[size] = 0;
}

void CSS::css_parser::splitByToken()
{
    string tempToken;


    for (size_t i = 0; i < code.size(); i++)
    {
        skipComment(i);
        skipExcessSymbols(i);

        auto& symbol = code[i];

        if (isSplitSymbol(symbol))
        {

            if (!tempToken.empty())
            {
                tokens.push_back(tempToken);
                tempToken.clear();
            }

            if (symbol != ' ')
            {
                const string symbolToken(1, symbol);

                tokens.push_back(symbolToken);
            }

            continue;
        }

        tempToken += symbol;
    }

    if (!tempToken.empty())
    {
        tokens.push_back(tempToken);
        tempToken.clear();
    }
}

bool CSS::css_parser::isSplitSymbol(const char& symbol)
{
    return  symbol == ' ' || symbol == ':' ||
            symbol == ';' || symbol == '{' ||
            symbol == '}' || symbol == '(' ||
            symbol == ')';
}

void CSS::css_parser::splitByBlock()
{
    vector<string> tempBlock;

    bool startBlock = false;

    for (auto& token : tokens)
    {
        /** start of block*/
        if (whatIsToken(token) == TokenType::CLASSNAME)
        {
            tempBlock.push_back(token);

            startBlock = true;
            continue;
        }

        if (startBlock)
        {
            if (token == "}") /** end of block */
            {
                tempBlock.push_back(token);
                blocks.push_back(tempBlock);
                tempBlock.clear();

                startBlock = false;
            }
            else
            {
                tempBlock.push_back(token);
            }
        }

    }
}

CSS::TokenType CSS::css_parser::whatIsToken(const std::string_view& token_)
{
    if (token_.size() > 1 && token_[0] == '.')
    {
        return TokenType::CLASSNAME;
    }
    else if (token_ == ":")
    {
        return TokenType::COLON;
    }
    else if (token_ == ";")
    {
        return TokenType::SEMICOLON;
    }
    else if (token_ == "{")
    {
        return TokenType::LBRA;
    }
    else if (token_ == "}")
    {
        return TokenType::RBRA;
    }
    else if (token_ == "(")
    {
        return TokenType::LPAR;
    }
    else if (token_ == ")")
    {
        return TokenType::RPAR;
    }
    else if (token_ == "hover" || token_ == "active" || token_ == "focus")
    {
        return TokenType::PSEUDO;
    }
    else
    {
        return TokenType::FIELD;
    }
}

void CSS::css_parser::syntaxParseOneBlock(std::vector<std::string>& block_)
{
    string attribute;
    string pseudo;

    vector<string> values;

    css_block block_css;
    css_block_state block_css_state;

    string identifier;
    state state;
    state_next next_state;

    next_state = state_next::NEXT_TOKEN_IS_IDENTIFICATOR;
    identifier = block_[0];

    if (whatIsToken(identifier) != TokenType::CLASSNAME)
    {
        cout << "ERROR: The first parameter in the block must be either a class identifier!" << endl;
        return;
    }


    state = state::THIS_TOKEN_IS_IDENTIFICATOR;
    next_state = state_next::NEXT_TOKEN_IS_LBRA_OR_COLON;


    for (size_t i = 1; i < block_.size(); i++)
    {
        auto& token = block_[i];

        TokenType type = whatIsToken(token);

        switch (next_state)
        {
            case CSS::state_next::NEXT_TOKEN_IS_LBRA_OR_COLON:
            {
                if (type != TokenType::LBRA && type != TokenType::COLON)
                {
                    cout << "ERROR: next token is invalid! Line " << __LINE__ << endl;
                }
                break;
            }

            case CSS::state_next::NEXT_TOKEN_IS_SEMICOLON:
            {
                if (type != TokenType::SEMICOLON)
                {
                    cout << "ERROR: next token is invalid! Line " << __LINE__ << endl;
                }
                break;
            }

            case CSS::state_next::NEXT_TOKEN_IS_COLON:
            {
                if (type != TokenType::COLON)
                {
                    cout << "ERROR: next token is invalid! Token: " << token << " Line " << __LINE__ << endl;
                }
                break;
            }

            case CSS::state_next::NEXT_TOKEN_IS_PSEUDO:
            {
                if (type != TokenType::PSEUDO)
                {
                    cout << "ERROR: next token is invalid! Line " << __LINE__ << endl;
                }
                break;
            }

            case CSS::state_next::NEXT_TOKEN_IS_FIELD:
            {
                if (type != TokenType::FIELD && type != TokenType::RBRA)
                {
                    cout << "ERROR: next token is invalid! Line " << __LINE__ << endl;
                }
                break;
            }

            default:
                break;
        }


        switch (type)
        {
            case TokenType::COLON:
            {
                if (state == state::THIS_TOKEN_IS_IDENTIFICATOR)
                {
                    state = state::THIS_TOKEN_IS_PSEUDO;
                    next_state = state_next::NEXT_TOKEN_IS_PSEUDO;
                }
                else if (state == state::THIS_TOKEN_IS_ATTRIBUTE)
                {
                    state = state::THIS_TOKEN_IS_VALUE;
                    next_state = state_next::NEXT_TOKEN_IS_FIELD;
                }

                break;
            }

            case TokenType::SEMICOLON:
            {

                if (values.size() > 1)
                {
                    syntaxParseIfComplexValueStatic(attribute, values, &block_css_state);
                }
                else if (values.size() == 1)
                {
                    const auto value = values[0];
                    block_css_state.set(attribute, value);
                }


                values.clear();



                state = state::THIS_TOKEN_IS_ATTRIBUTE;
                next_state = state_next::NEXT_TOKEN_IS_FIELD;

                break;
            }

            case TokenType::PSEUDO:
            {
                pseudo = token;

                state = state::THIS_TOKEN_IS_ANY;
                next_state = state_next::NEXT_TOKEN_IS_ANY;
                break;
            }

            case TokenType::LBRA:
            {
                block_css.name(identifier);

                state = state::THIS_TOKEN_IS_ATTRIBUTE;
                next_state = state_next::NEXT_TOKEN_IS_FIELD;
                break;
            }

            case TokenType::RBRA:
            {
                if (pseudo == "hover")
                {
                    block_css.hover(block_css_state);
                }
                else if (pseudo == "active")
                {
                    block_css.active(block_css_state);
                }
                else if (pseudo == "focus")
                {
                    block_css.focus(block_css_state);
                }
                else
                {
                    block_css.normal(block_css_state);
                }

                if (css_blocks.find(identifier) != css_blocks.end())
                {
                    css_blocks[identifier].mergeWith(block_css);
                }
                else
                {
                    css_blocks.insert(std::make_pair(identifier, block_css));
                }

                state = state::THIS_TOKEN_IS_ANY;
                next_state = state_next::NEXT_TOKEN_IS_ANY;
                break;
            }

            case TokenType::FIELD:
            {
                if (state == state::THIS_TOKEN_IS_VALUE)
                {
                    while (i < block_.size() && block_[i] != ";")
                    {
                        values.push_back(block_[i]);
                        i++;
                    }

                    i--;

                    next_state = state_next::NEXT_TOKEN_IS_SEMICOLON;
                }
                else if (state == state::THIS_TOKEN_IS_ATTRIBUTE)
                {
                    attribute.clear();
                    attribute = token;

                    next_state = state_next::NEXT_TOKEN_IS_COLON;
                }

                break;
            }

            default:
                break;
        }

    }
}

void CSS::css_parser::syntaxParseIfComplexValueStatic(const std::string& attribute_, const vector<string>& values_,
                                                      CSS::css_block_state* block_state_)
{
    if (block_state_ == nullptr)
        return;


    if (attribute_ == "border-top" || attribute_ == "border-bottom" ||
        attribute_ == "border-left" || attribute_ == "border-right")
    {
        if (values_.size() != 3)
        {
            cout << "ERROR: attribute_ " << attribute_ << " must have 3 values, but only "
                 << values_.size() << " passed!" << endl;
            return;
        }

        const int border_size = Utils::to_integer(values_[0]);
        string border_type = values_[1];

        if (border_type != "solid")
        {
            cout << "ERROR: border type " << border_type << " not found! Set to solid" << endl;
            border_type = "solid";
        }

        const Color border_color(values_[2]);


        block_state_->set(attribute_ + "-size", border_size);
        block_state_->set(attribute_ + "-type", border_type);
        block_state_->set(attribute_ + "-color", border_color);

        return;
    }


    if (attribute_ == "width" || attribute_ == "height" ||
        attribute_ == "top"   || attribute_ == "left")
    {
        const string result = values_[2] + values_[3] + values_[4];

        block_state_->set(attribute_, result);
        return;
    }

    if (attribute_ == "background-image")
    {
        const string result = values_[2];

        block_state_->set(attribute_, result);
        return;
    }
}

void CSS::css_parser::mergeStyleComponent()
{
    for (auto& [id, block] : css_blocks)
    {
        block.focus().mergeWithBaseIs(block.normal());
        block.hover().mergeWithBaseIs(block.focus());
        block.active().mergeWithBaseIs(block.hover());
    }
}

void CSS::css_parser::skipComment(size_t& i_)
{
    bool comment = false;

    if (code[i_] == '/' && code[i_ + 1] == '*')
    {
        comment = true;
    }

    if (comment)
    {
        while (comment)
        {
            if (code[i_] == '*' && code[i_ + 1] == '/')
            {
                comment = false;
                i_++;
            }
            i_++;
        }
    }
}

void CSS::css_parser::skipExcessSymbols(size_t& i_)
{
    char current_symbol = code[i_];

    while (current_symbol == '\n' || current_symbol == '\r')
    {
        i_++;
        current_symbol = code[i_];
    }

    if (current_symbol == ' ')
    {
        if (i_ + 1 < code.size() && code[i_ + 1] == ' ')
        {
            i_++;
        }
    }
}

void CSS::css_parser::syntaxParse()
{
    for (auto& block : blocks)
    {
        syntaxParseOneBlock(block);
    }
}

void CSS::css_parser::updateCSS()
{
    for (auto&[id, block] : css_blocks)
    {
        css_parent->add(block);
    }
}