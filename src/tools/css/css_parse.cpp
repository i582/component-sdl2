#include "css_parse.h"
#include "css.h"


CSS::css_parser::css_parser(const string& file_path, CSS::css* css_parent)
{
    if (css_parent == nullptr)
    {
        cout << "ERROR: css_parent is nullptr" << endl;
        return;
    }

    this->file = nullptr;
    this->css_parent = css_parent;
    this->file_path = file_path;

    openFile(); 
}

CSS::css_parser::css_parser(const string& code, bool is_code, CSS::css* css_parent)
{
    if (css_parent == nullptr)
    {
        cout << "ERROR: css_parent is nullptr" << endl;
        return;
    }

    this->file = nullptr;
    this->css_parent = css_parent;
    this->code = code;

    deleteSpaceInCode();
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
    file = fopen(file_path.c_str(), "r");

    if (file == nullptr)
    {
        cout << "Error: css file not found!" << endl;
        return;
    }

    while (true)
    {
        if (getc(file) == EOF) break;
        else fseek(file, -1, SEEK_CUR);

        deleteExcess();
        code += getc(file);

    }

    fclose(file);

#ifdef _DEBUG_VERSION_
    std::cout << code << std::endl;
#endif // _DEBUG_VERSION_

}

void CSS::css_parser::deleteExcess()
{
    char symbol = -1;

    while (true)
    {
        symbol = getc(file);

        if (symbol == ' ')
        {

            symbol = getc(file);
            while (symbol == ' ' && symbol != EOF)
            {
                symbol = getc(file);
            }

            //fseek(lib, -1, SEEK_CUR);

        }


        if (!(symbol == '\t' || symbol == '\n' || symbol == '\r'))
        {
            fseek(file, -1, SEEK_CUR);
            break;
        }


        if (symbol == EOF)
            break;
    }
}

void CSS::css_parser::deleteSpaceInCode()
{
    string newCode;

    for (auto& symbol : code)
    {
        if (symbol != ' ')
        {
            newCode += symbol;
        }
    }

    code = newCode;
}

bool CSS::css_parser::isSplitSymbol(char symbol)
{
    return symbol == ':' || symbol == ';' || symbol == '{' || symbol == '}';
}

CSS::TokenType CSS::css_parser::whatIsToken(const string& token)
{
    if (token[0] == '.' && token.size() > 1)
    {
        return TokenType::CLASSNAME;
    }
    else if (token == ":")
    {
        return TokenType::COLON;
    }
    else if (token == ";")
    {
        return TokenType::SEMICOLON;
    }
    else if (token == "{")
    {
        return TokenType::LBRA;
    }
    else if (token == "}")
    {
        return TokenType::RBRA;
    }
    else if (token == "(")
    {
        return TokenType::LPAR;
    }
    else if (token == ")")
    {
        return TokenType::RPAR;
    }
    else if (token == "hover" || token == "active" || token == "focus")
    {
        return TokenType::PSEUDO;
    }
    else
    {
        return TokenType::FIELD;
    }
}

void CSS::css_parser::skipComment(size_t& i)
{

    bool comment = false;

    if (code[i] == '/' && code[i + 1] == '*')
    {
        comment = true;
    }

    if (comment)
    {
        while (comment)
        {
            if (code[i] == '*' && code[i + 1] == '/')
            {
                comment = false;
                i++;
            }
            i++;
        }
    }
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

void CSS::css_parser::splitByToken()
{
    string tempToken;

    for (size_t i = 0; i < code.size(); i++)
    {
        this->skipComment(i);


        auto& symbol = code[i];

        if (isSplitSymbol(symbol))
        {

            if (!tempToken.empty())
            {
                tokens.push_back(tempToken);
                tempToken.clear();
            }

            string symbolToken;
            symbolToken += symbol;

            tokens.push_back(symbolToken);

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

void CSS::css_parser::syntaxParseOneBlock(vector<string>& block)
{
    string attribute;
    string value;
    string pseudo;


    css_block block_css;
    css_block_state block_css_state;

    string identificator;
    state state;
    state_next next_state;

    next_state = state_next::NEXT_TOKEN_IS_IDENTIFICATOR;
    identificator = block[0];

    if (next_state == state_next::NEXT_TOKEN_IS_IDENTIFICATOR &&
        whatIsToken(identificator) != TokenType::CLASSNAME)
    {
        cout << "ERROR: The first parameter in the block must be either a class identifier!" << endl;
        return;
    }


    state = state::THIS_TOKEN_IS_IDENTIFICATOR;
    next_state = state_next::NEXT_TOKEN_IS_LBRA_OR_COLON;


    for (size_t i = 1; i < block.size(); i++)
    {
        auto& token = block[i];

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
                    cout << "ERROR: next token is invalid! Line " << __LINE__ << endl;
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
                syntaxParseIfComplexValue(attribute, value, &block_css_state);


                block_css_state.set(attribute, CSS::css_attribute::get(attribute, value));

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
                block_css.name(identificator);

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

                if (css_blocks.find(identificator) != css_blocks.end())
                {
                    css_blocks[identificator].mergeWith(block_css);
                }
                else
                {
                    css_blocks.insert(std::make_pair(identificator, block_css));
                }

                state = state::THIS_TOKEN_IS_ANY;
                next_state = state_next::NEXT_TOKEN_IS_ANY;
                break;
            }

            case TokenType::FIELD:
            {
                if (state == state::THIS_TOKEN_IS_VALUE)
                {
                    value.clear();
                    value = token;

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

void CSS::css_parser::syntaxParse()
{
    for (auto& block : blocks)
    {
        syntaxParseOneBlock(block);
    }
}

void CSS::css_parser::mergeStyleComponent()
{
    for (auto&[id, block] : css_blocks)
    {
        block.focus().mergeWithBaseIs(block.normal());
        block.hover().mergeWithBaseIs(block.focus());
        block.active().mergeWithBaseIs(block.hover());
    }
}

void CSS::css_parser::updateCSS()
{
    for (auto&[id, block] : css_blocks)
    {
        css_parent->add(block);
    }
}

void CSS::css_parser::syntaxParseIfComplexValue(string attribute, string value, CSS::css_block_state* block)
{
    if (block == nullptr)
        return;


    if (attribute == "border-top" || attribute == "border-bottom" ||
        attribute == "border-left" || attribute == "border-right")
    {

        value.replace(value.find("px"), 2, " ");
        value.replace(value.find('#'), 1, " ");

        vector<string>* tokens = Utils::split(value, ' ');
        if (tokens->size() != 3)
        {
            cout << "ERROR: attribute " << attribute << " must have 3 values, but only "
                 << tokens->size() << " passed! Value: " << value << endl;
            return;
        }

        int border_size = Utils::to_integer(tokens->at(0));
        string border_type = tokens->at(1);

        if (border_type != "solid")
        {
            cout << "ERROR: border type " << border_type << " not found! Set to solid" << endl;
            border_type = "solid";
        }

        Color border_color("#" + tokens->at(2));


        block->set(attribute + "-size", border_size);
        block->set(attribute + "-type", border_type);
        block->set(attribute + "-color", border_color);

        return;
    }

}
