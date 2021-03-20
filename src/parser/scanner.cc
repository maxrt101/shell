#include "parser/scanner.h"

#include <cctype>
#include <string>


using namespace parser; // to shorten the code; shouldn't be a problem


struct ScannerState {
  //const char* start;
  //const char* current;
  std::string::iterator start;
  std::string::iterator current;
} scanner_state;


static bool IsAtEnd() {
  return *scanner_state.current == '\0';
}

static Token MakeToken(TokenType type) {
  Token token;
  token.type = type;
  token.str = std::string(scanner_state.start, scanner_state.current);
  return token;
}

static Token ErrorToken(std::string msg) {
  return Token(TOKEN_ERROR, msg);
}

static char Advance() {
  scanner_state.current++;
  return scanner_state.current[-1];
}

static bool Match(char expected) {
  if (IsAtEnd()) return false;
  if (*scanner_state.current != expected) return false;

  scanner_state.current++;
  return true;
}

static char Peek() {
  return *scanner_state.current;
}

static char PeekNext() {
  return scanner_state.current[1];
}

static void SkipWhitespace() {
  for (;;) {
    char c = Peek();
    switch (c) {
      case ' ':
      case '\r':
      case '\t':
      case '\n':
        Advance();
        break;
      default:
        return;
    }
  }
}

static Token String() {
  while (Peek() != '"' && !IsAtEnd()) {
    Advance();
  }

  if (IsAtEnd()) return ErrorToken("Unterminated string.");

  Advance();
  return MakeToken(TOKEN_STRING);
}

static Token Number() {
  while (isdigit(Peek())) Advance();
  return MakeToken(TOKEN_NUMBER);
}

static Token Word() {
  // This matches anything, except for shell control charactes
  // while (isalpha(Peek()) || isdigit(Peek()) || Peek() == '_') Advance();
  while (!IsAtEnd()
      && Peek() != ' ' && Peek() != '\n'
      && Peek() != '|' && Peek() != '&'
      && Peek() != '>' && Peek() != '<'
      && Peek() != '(' && Peek() != ')'
      && Peek() != '{' && Peek() != '}') Advance();
  return MakeToken(TOKEN_IDENTIFIER);
}

static Token ScanToken() {
  SkipWhitespace();

  scanner_state.start = scanner_state.current;

  if (IsAtEnd()) return MakeToken(TOKEN_EOF);

  char c = Advance();

  if (isalpha(c)) return Word();
  if (isdigit(c)) return Number();

  switch (c) {
    case '(': return MakeToken(TOKEN_LEFT_PAREN);
    case ')': return MakeToken(TOKEN_RIGHT_PAREN);
    case '{': return MakeToken(TOKEN_LEFT_BRACE);
    case '}': return MakeToken(TOKEN_RIGHT_BRACE);
    case ';': return MakeToken(TOKEN_SEMICOLON);
    // case '.': return MakeToken(TOKEN_DOT);
    case '>': return MakeToken(TOKEN_GREATER);
    case '<': return MakeToken(TOKEN_LESS);
    case '|': return MakeToken(TOKEN_PIPE);
    case '&': return MakeToken(TOKEN_AMPERSAND);
    case '"': return String();
    default:  return Word();
    // return ErrorToken("Unexpected characted");
  }
}

/*TokenArray*/ std::vector<parser::Token> parser::SplitLine(std::string& line) {
  scanner_state.start = line.begin();
  scanner_state.current = line.begin();

  std::vector<parser::Token> tokens;

  while (!IsAtEnd()) {
    Token token = ScanToken();
    if (token.type == TOKEN_EOF || token.type == TOKEN_ERROR) break;
    tokens.push_back(token);
  }

  return tokens;
}

