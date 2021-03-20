#ifndef SHELL_PARSER_SCANNER_H_
#define SHELL_PARSER_SCANNER_H_

#include <vector>

#include "parser/tokens.h"


namespace parser {

/** SplitLine(line) - splits line into tokens
 * On error - last token of returning vector will be TOKEN_ERROR
 * .str of that token will be the error message
 */
/*TokenArray*/ std::vector<Token> SplitLine(std::string& line);

} // namespace parser

#endif

