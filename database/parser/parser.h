#include "../tokenizer/token_list.h"
#include <stdbool.h>
#include <stddef.h>

// void printAllTokens(tokenListCTX *ctx);

bool check(size_t tokenType, size_t tokenTypeToBeChecked);
void advance(tokenListCTX *tokenListCTX);
void showIndex(tokenListCTX *tokenListCTX);
void parse(tokenListCTX *tokenListCTX);

void sqlStatement(tokenListCTX *tokenListCTX);
void insertStatement(tokenListCTX *tokenListCTX);
void updateStatement(tokenListCTX *tokenListCTX);
void deleteStatement(tokenListCTX *tokenListCTX);

// SELECT
void selectStatement(tokenListCTX *tokenListCTX);
void selectList(tokenListCTX *tokenListCTX);
void tableList(tokenListCTX *tokenListCTX);
void whereClause(tokenListCTX *tokenListCTX);
void condition(tokenListCTX *tokenListCTX);
void expression(tokenListCTX *tokenListCTX);
void comparisonOp(tokenListCTX *tokenListCTX);
void logicalOp(tokenListCTX *tokenListCTX);
