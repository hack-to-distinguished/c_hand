#include "../tokenizer/token_list.h"
#include "./nodes/select_list.h"
#include "./nodes/statement_nodes.h"
#include <stdbool.h>
#include <stddef.h>

// PARSE FUNCS
SQLStatement *parseSQLStatment(tokenListCTX *tokenListCTX);
SelectStatement *parseSelectStatement(tokenListCTX *tokenListCTX);
InsertStatement *parseInsertStatement(tokenListCTX *tokenListCTX);
UpdateStatement *parseUpdateStatement(tokenListCTX *tokenListCTX);
DeleteStatement *parseDeleteStatement(tokenListCTX *tokenListCTX);
ExitStatement *parseExitStatement(tokenListCTX *tokenListCTX);
SelectList *parseSelectList(tokenListCTX *tokenListCTX);

// UTIL FUNCS
void parse(tokenListCTX *tokenListCTX);
void syntaxError(const char *message);
bool check(size_t tokenType, size_t tokenTypeToBeChecked);
void advance(tokenListCTX *tokenListCTX);
void consumeToken(size_t tokenType, size_t tokenTypeToBeChecked,
                  tokenListCTX *tokenListCTX);
void showIndex(tokenListCTX *tokenListCTX);
Token peekToken(tokenListCTX *tokenListCTX);
