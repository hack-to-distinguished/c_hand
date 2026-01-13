#include "../tokenizer/token_list.h"
#include "nodes.h"
#include <stdbool.h>
#include <stddef.h>

void syntaxError(const char *message);
bool check(size_t tokenType, size_t tokenTypeToBeChecked);
void advance(tokenListCTX *tokenListCTX);
void consumeToken(size_t tokenType, size_t tokenTypeToBeChecked,
                  tokenListCTX *tokenListCTX);
void showIndex(tokenListCTX *tokenListCTX);
void parse(tokenListCTX *tokenListCTX);
Token peekToken(tokenListCTX *tokenListCTX);

// TODO: here will have the definitions for the parsing functions represnting
// each grammar rule
SQLStatement *parseSQLStatment(tokenListCTX *tokenListCTX);
SelectStatement *parseSelectStatement(tokenListCTX *tokenListCTX);
InsertStatement *parseInsertStatement(tokenListCTX *tokenListCTX);
UpdateStatement *parseUpdateStatement(tokenListCTX *tokenListCTX);
DeleteStatement *parseDeleteStatement(tokenListCTX *tokenListCTX);
ExitStatement *parseExitStatement(tokenListCTX *tokenListCTX);
