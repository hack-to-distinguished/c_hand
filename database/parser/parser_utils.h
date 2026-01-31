#include "../tokenizer/token_list.h"
#include "ast/ast.h"
#include <stdbool.h>
#include <stddef.h>

// PARSE FUNCS
ASTNode *parseSQLStatment(tokenListCTX *tokenListCTX);
ASTNode *parseExitStatement(tokenListCTX *tokenListCTX);
ASTNode *parseSelectStatement(tokenListCTX *tokenListCTX);
ASTNode *parseSelectList(tokenListCTX *tokenListCTX);
ASTNode *parseSimpleExpression(tokenListCTX *tokenListCTX);
ASTNode *parseTerm(tokenListCTX *tokenListCTX);
ASTNode *parseAdditiveOperator(tokenListCTX *tokenListCTX);
ASTNode *parseFactor(tokenListCTX *tokenListCTX);
ASTNode *parseMultiplicativeOperator(tokenListCTX *tokenListCTX);
ASTNode *parseQualifiedIdentifier(tokenListCTX *tokenListCTX);
ASTNode *parseLiteral(tokenListCTX *tokenListCTX);
ASTNode *parseFunctionCall(tokenListCTX *tokenListCTX);

// UTIL FUNCS
void parse(tokenListCTX *tokenListCTX);
void syntaxError(const char *message);
bool check(size_t tokenType, size_t tokenTypeToBeChecked);
void advance(tokenListCTX *tokenListCTX);
void decrement(tokenListCTX *tokenListCTX);
void consumeToken(size_t tokenType, size_t tokenTypeToBeChecked,
                  tokenListCTX *tokenListCTX);
void showIndex(tokenListCTX *tokenListCTX);
Token peekToken(tokenListCTX *tokenListCTX);
