#pragma once


#include "css-attributes.h"

namespace CSS
{

using std::string;
using std::vector;
using std::map;

using std::cout;
using std::endl;

enum class TokenType
{
	ID, // #*
	CLASSNAME, // .*
	COLON, // :
	SEMICOLON, // ;
	LBRA, // {
	RBRA, // }

	LPAR, // (
	RPAR, // )

	PSEUDO, // hover or active

	FIELD
};

enum State
{
	NEXT_TOKEN_IS_ID,
	NEXT_TOKEN_IS_CLASSNAME,

	NEXT_TOKEN_IS_LBRA,

	NEXT_TOKEN_IS_COLON,
	NEXT_TOKEN_IS_SEMICOLON,

	NEXT_TOKEN_IS_ATTRIBUTE,
	NEXT_TOKEN_IS_VALUE,

	NEXT_TOKEN_IS_PSEUDO,

	NEXT_TOKEN_IS_COMPLEX_VALUE,

	END_BLOCK,

	START_PARSE
};

enum class state
{
	THIS_TOKEN_IS_IDENTIFICATOR,

	THIS_TOKEN_IS_COLON,
	THIS_TOKEN_IS_SEMICOLON,

	THIS_TOKEN_IS_PSEUDO,

	THIS_TOKEN_IS_ATTRIBUTE,
	THIS_TOKEN_IS_VALUE,

	THIS_TOKEN_IS_ANY,
};

enum class state_next
{
	NEXT_TOKEN_IS_IDENTIFICATOR,

	NEXT_TOKEN_IS_LBRA_OR_COLON,

	NEXT_TOKEN_IS_SEMICOLON,
	NEXT_TOKEN_IS_COLON,

	NEXT_TOKEN_IS_PSEUDO,

	NEXT_TOKEN_IS_FIELD,

	NEXT_TOKEN_IS_ANY
};

class css;

class css_block;

class css_block_state;

class css_parser
{
private:
	string file_path;

	FILE* file;
	string code;

	vector<string> tokens;

	vector<vector<string> > blocks;

	map <string, CSS::css_block> css_blocks;

	CSS::css* css_parent;

public:
	css_parser(string file_path, CSS::css* css_parent);
	css_parser(string code, bool isCode, CSS::css* css_parent);

public:
	void parse();

private:
	void openFile();
	void deleteExcess();
	void deleteSpaceInCode();

private:
	bool isSplitSymbol(char symbol);
	TokenType whatIsToken(string token);
	

private:
	void skipComment(size_t& i);

private:
	void splitByBlock();
	void splitByToken();
	void syntaxParseOneBlock(vector<string>& block);
	void syntaxParse();
	void mergeStyleComponent();
	void updateCSS();


	void syntaxParseIfComplexValue(string attribute, string value, CSS::css_block_state* block_state);
};

}