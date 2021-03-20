#ifndef SHELL_PARSER_TOKENS_H_
#define SHELL_PARSER_TOKENS_H_

#include <string_view>
#include <iostream>
#include <iterator>
#include <cstddef>

namespace parser {

/* Represents all possible token types */
enum TokenType : char {
  TOKEN_EOF = 0,      // EOF

  TOKEN_LEFT_PAREN,   // '('
  TOKEN_RIGHT_PAREN,  // ')'
  TOKEN_LEFT_BRACE,   // '{'
  TOKEN_RIGHT_BRACE,  // '}'
  TOKEN_SEMICOLON,    // ';'
  TOKEN_DOT,          // '.'
  TOKEN_PIPE,         // '|'
  TOKEN_GREATER,      // '>'
  TOKEN_LESS,         // '>'
  TOKEN_AMPERSAND,    // '&'
  TOKEN_EQUALS,       // '='
  TOKEN_DOLLAR,       // '$'
  TOKEN_NESTED,       // '$(..)'
  TOKEN_IDENTIFIER,   // \w+
  TOKEN_STRING,       // \".*\"
  TOKEN_NUMBER,       // \d+
  TOKEN_ERROR
};


/* Represents token */
struct Token {
  TokenType type;
  std::string str;

  inline Token() : type(TOKEN_EOF) {}
  inline Token(TokenType type, std::string str) : type(type), str(str) {}
  inline Token(const Token& rhs) {
    type = rhs.type;
    str = std::string(rhs.str);
  }
};

} // namespace parser

#endif

