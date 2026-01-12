#include "../tokenizer/token_list.h"
#include "nodes.h"
#include <stdbool.h>
#include <stddef.h>

bool check(size_t tokenType, size_t tokenTypeToBeChecked);
void advance(tokenListCTX *tokenListCTX);
void showIndex(tokenListCTX *tokenListCTX);
void parse(tokenListCTX *tokenListCTX);
Token *peekToken(tokenListCTX *tokenListCTX);

// TODO: here will have the definitions for the parsing functions represnting
// each grammar rule
SQLStatement *parseSQLStatment(tokenListCTX *tokenListCTX);
SelectStatement *parseSelectStatement();
InsertStatement *parseInsertStatement();
UpdateStatement *parseUpdateStatement();
DeleteStatement *parseDeleteStatement();
ExitStatement *parseExitStatement();
