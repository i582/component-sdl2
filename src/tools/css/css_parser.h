#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

#include "color/color.h"
#include "utils/utils.h"

#include "css_variant/css_variant.h"

namespace CSS
{

    using std::string;
    using std::string_view;
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

        PSEUDO, // hover or active or focus

        FIELD
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

        string code;

        vector<string> tokens;
        vector< vector<string> > blocks;
        map<string, css_block> css_blocks;

        css* css_parent;

    public:
        css_parser(const string& file_path_, css* css_parent_);
        css_parser(const string& code_, bool is_code, css* css_parent_);

    private:
        /**
         * @brief Opens a file for parsing.
         */
        void openFile();

        /**
         * @brief Function to skip comments in code.
         * @param i_ The function takes the current index in a row in the splitByToken
         * method and adds the index to it until the comment ends.
         */
        void skipComment(size_t& i_);

        /**
         * @brief Function to skip extra characters (space, \r, \n) in code.
         * @param i_ The function takes the current index in a row in the splitByToken method
         */
        void skipExcessSymbols(size_t& i_);

        /**
         * @brief Function split the source code into tokens.
         */
        void splitByToken();

        /**
         * @brief Function splits tokens into separate blocks.
         */
        void splitByBlock();

        /**
         * @brief Function parses blocks syntactically
         */
        void syntaxParse();

        /**
         * @brief Function parses the block syntactically, creating blocks with
         * different states (an almost ready-made data structure)
         * @param block_ Current block
         */
        void syntaxParseOneBlock(vector<string>& block_);

        /**
         * @brief Function combining various pseudo-classes of one component into a single unit
         */
        void mergeStyleComponent();

        /**
         * @brief The resulting function that updates the styles.
         */
        void updateCSS();

        /**
         * @brief Function that determines what kind of current was transferred to it.
         * @param token_ Token
         * @return Token type
         */
        static TokenType whatIsToken(const string_view& token_);

        /**
         * @brief Function that checks whether a given character is a delimiter.
         * @param symbol
         * @return true|false
         */
        static bool isSplitSymbol(const char& symbol);

        /**
         * @brief Function for parsing complex expressions (for example, border-top),
         * while the function sets additional ones in the final style block that describe
         * the styles of the complex property that it parses.
         * @param attribute_ Attribute
         * @param value_ Value
         * @param block_state_ Current block
         */
        static void syntaxParseIfComplexValueStatic(const string& attribute_, const vector<string>& values_, CSS::css_block_state* block_state_);

    public:
        /**
         * @brief The main method is novice parsing.
         */
        void parse();

    };

}