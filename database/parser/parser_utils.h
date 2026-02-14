#include "../tokenizer/token_list.h"
#include "ast/ast.h"
#include <stdbool.h>
#include <stddef.h>

void destroyASTNode(ASTNode *node);

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
ASTNode *parseArgList(tokenListCTX *tokenListCTX);
ASTNode *parseTableList(tokenListCTX *tokenListCTX);
ASTNode *parseWhereClause(tokenListCTX *tokenListCTX);
ASTNode *parseCondition(tokenListCTX *tokenListCTX);
ASTNode *parseExpression(tokenListCTX *tokenListCTX);
ASTNode *parseComparisonOperator(tokenListCTX *tokenListCTX);
ASTNode *parseOrCondition(tokenListCTX *tokenListCTX);
ASTNode *parseAndCondition(tokenListCTX *tokenListCTX);
ASTNode *parseBooleanFactor(tokenListCTX *tokenListCTX);
ASTNode *parseComparison(tokenListCTX *tokenListCTX);
ASTNode *parseOrderClause(tokenListCTX *tokenListCTX);
ASTNode *parseSortOrder(tokenListCTX *tokenListCTX);
ASTNode *parseInsertStatement(tokenListCTX *tokenListCTX);
ASTNode *parseColumnList(tokenListCTX *tokenListCTX);
ASTNode *parseValueList(tokenListCTX *tokenListCTX);
ASTNode *parseDeleteStatement(tokenListCTX *tokenListCTX);

// UTIL FUNCS
bool containsCondition(tokenListCTX *tokenListCTX);
void parse(tokenListCTX *tokenListCTX);
void syntaxError(const char *message);
bool check(size_t tokenType, size_t tokenTypeToBeChecked);
void advance(tokenListCTX *tokenListCTX);
void decrement(tokenListCTX *tokenListCTX);
void consumeToken(size_t tokenType, size_t tokenTypeToBeChecked,
                  tokenListCTX *tokenListCTX);
void showIndex(tokenListCTX *tokenListCTX);
Token peekToken(tokenListCTX *tokenListCTX);
