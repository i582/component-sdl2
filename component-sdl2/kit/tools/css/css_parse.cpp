#include "css_parse.h"
#include "css.h"


CSS::css_parser::css_parser(string file_path, CSS::css* css_parent)
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

CSS::css_parser::css_parser(string code, bool isCode, CSS::css* css_parent)
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

	while (1)
	{
		symbol = getc(file);

		if (symbol == ' ')
		{

			symbol = getc(file);
			while (symbol == ' ' && symbol != EOF)
			{
				symbol = getc(file);
			}

			//fseek(file, -1, SEEK_CUR);

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
	return symbol == ':' || symbol == '.' || symbol == '#' || symbol == ';' || symbol == '{' || symbol == '}'
		|| symbol == '(' || symbol == ')';
}

CSS::TokenType CSS::css_parser::whatIsToken(string token)
{
	if (token == ".")
	{
		return TokenType::CLASSNAME_SIGN;
	}
	else if (token == "#")
	{
		return TokenType::ID_SIGN;
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
	else
	{
		return TokenType::FIELD;
	}
}

void CSS::css_parser::splitByBlock()
{
	vector<string> tempBlock;

	for (auto& token : tokens)
	{
		if (token != "}")
		{
			tempBlock.push_back(token);
		}
		else
		{
			tempBlock.push_back(token);
			blocks.push_back(tempBlock);
			tempBlock.clear();
		}
		
	}

}

void CSS::css_parser::splitByToken()
{
	string tempToken;


	int i = 0;
	for (auto& symbol : code)
	{

		if (isSplitSymbol(symbol))
		{
			if (symbol == '.' && i != 0)
			{
				char symb = code[i - 1];
				if (symb >= '0' && symb <= '9')
				{
					tempToken += symbol;
					i++;

					continue;
				}
			}

			if (!tempToken.empty())
			{
				tokens.push_back(tempToken);
				tempToken.clear();
			}

			string symbolToken;
			symbolToken += symbol;

			tokens.push_back(symbolToken);

			i++;
			continue;
		}

		tempToken += symbol;
		i++;
	}



}

void CSS::css_parser::syntaxParseOneBlock(vector<string>& block)
{
	State nowState = State::START_PARSE;

	int countAttributesWithoutValue = 0;

	

	string idOrClassName;

	string attribute;
	string value;
	string pseudo;


	css_block block_css;
	css_block_state block_css_state;


	

	for (auto& token : block)
	{
		TokenType nowTokenType = whatIsToken(token);

		if (nowState == State::NEXT_TOKEN_IS_COMPLEX_VALUE)
		{
		
			if (nowTokenType == TokenType::RPAR)
			{
				nowState = State::NEXT_TOKEN_IS_SEMICOLON;

#ifdef _DEBUG_VERSION_
				cout << "End LPAR block" << endl;
				cout << "Value = " << value << endl;
#endif // _DEBUG_VERSION_
			}
			else
			{
				value += token;
			}

			continue;
		}

		switch (nowTokenType)
		{
		case TokenType::ID_SIGN:
		{
			if (nowState == State::NEXT_TOKEN_IS_VALUE)
			{
				nowState = State::NEXT_TOKEN_IS_VALUE;
			}
			else
			{
				nowState = State::NEXT_TOKEN_IS_ID;

				idOrClassName += "#";
			}



			break;
		}

		case TokenType::CLASSNAME_SIGN:
		{
			if (nowState == State::NEXT_TOKEN_IS_VALUE)
			{
				nowState = State::NEXT_TOKEN_IS_VALUE;
			}
			else
			{
				nowState = State::NEXT_TOKEN_IS_CLASSNAME;

				idOrClassName += ".";

			}
			break;
		}

		case TokenType::COLON:
		{
			if (nowState == State::NEXT_TOKEN_IS_ID || nowState == State::NEXT_TOKEN_IS_CLASSNAME)
			{
				nowState = State::NEXT_TOKEN_IS_PSEUDO;
			}
			else
			{
				nowState = State::NEXT_TOKEN_IS_VALUE;
				value.clear();
			}

			

			break;
		}

		case TokenType::SEMICOLON:
		{
			if (countAttributesWithoutValue != 0)
			{
				cout << "ERROR: Attribute without value!" << endl;
				return;
			}

			if (attribute.find("color") != -1)
			{
				value = '#' + value;
			}


			syntaxParseIfComplexValue(attribute, value, &block_css_state);

			block_css_state.set(attribute, CSS::css_attribute::get(attribute, value));

		
			nowState = State::NEXT_TOKEN_IS_ATTRIBUTE;
			break;
		}

		case TokenType::LPAR:
		{
			nowState = State::NEXT_TOKEN_IS_COMPLEX_VALUE;

			value.clear();
			//value += token;
#ifdef _DEBUG_VERSION_
			cout << "Start LPAR block" << endl;
#endif // _DEBUG_VERSION_
			break;
		}

		case TokenType::LBRA:
		{
			nowState = State::NEXT_TOKEN_IS_ATTRIBUTE;

			block_css.name(idOrClassName);
#ifdef _DEBUG_VERSION_
			cout << "Start block" << endl;
#endif // _DEBUG_VERSION_
			break;
		}

		case TokenType::RBRA:
		{
			nowState = State::END_BLOCK;

#ifdef _DEBUG_VERSION_
			cout << "End block" << endl;
#endif // _DEBUG_VERSION_

		

			if (pseudo == "hover")
			{
				block_css.hover(block_css_state);
			}
			else if (pseudo == "active")
			{
				block_css.active(block_css_state);
			}
			else
			{
				block_css.normal(block_css_state);
			}

			if (css_blocks.find(idOrClassName) != css_blocks.end())
			{
				css_blocks[idOrClassName].mergeWith(block_css);

			}
			else
			{
				css_blocks.insert(std::make_pair(idOrClassName, block_css));
			}

			break;
		}

		case TokenType::FIELD:
		{
			switch (nowState)
			{
			case NEXT_TOKEN_IS_ID:
			{
				idOrClassName += token;

#ifdef _DEBUG_VERSION_
				cout << "This token is ID: " << token << endl;
				cout << "Block: " << idOrClassName << endl;
#endif // _DEBUG_VERSION_

				break;
			}

			case NEXT_TOKEN_IS_CLASSNAME:
			{
				idOrClassName += token;

#ifdef _DEBUG_VERSION_
				cout << "This token is CLASSNAME: " << token << endl;
				cout << "Block: " << idOrClassName << endl;
#endif // _DEBUG_VERSION_

				break;
			}

			case NEXT_TOKEN_IS_ATTRIBUTE:
			{
				countAttributesWithoutValue++;

#ifdef _DEBUG_VERSION_
				cout << "This token is ATTRIBUTE: " << token << endl;
#endif // _DEBUG_VERSION_

				attribute.clear();
				attribute = token;

				break;
			}

			case NEXT_TOKEN_IS_VALUE:
			{
#ifdef _DEBUG_VERSION_
				cout << "This token is VALUE: " << token << endl;
#endif // _DEBUG_VERSION_

				if (!value.empty())
				{
					value += ' ' + token;
				}
				else
				{
					countAttributesWithoutValue--;
					value = token;
				}

				

				break;
			}

			case NEXT_TOKEN_IS_PSEUDO:
			{
#ifdef _DEBUG_VERSION_
				cout << "This token is PSEUDO: " << token << endl;
#endif // _DEBUG_VERSION_

				if (token != "hover" && token != "active" && token != "focus")
				{
					cout << "ERROR: Undefined pseudo class " << token << "!" << endl;

					return;
				}

				pseudo = token;

				break;
			}


			default:
			{

				cout << "ERROR: Undefined token: " << token << endl;


				break;
			}
			}


			break;
		}

		default:break;
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
	for (auto& block : css_blocks)
	{
		block.second.hover().mergeWithBaseIs(block.second.normal());
		block.second.active().mergeWithBaseIs(block.second.hover());
	}
}
void CSS::css_parser::updateCSS()
{
	for (auto& block : css_blocks)
	{
		CSS::css_block block_raw(block.second.name(), true);

		block_raw.mergeWith(block.second);

		css_parent->add(block_raw);
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

		vector <string>* tokens = Utils::split(value, ' ');
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
