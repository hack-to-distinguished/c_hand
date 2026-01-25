#include "../parser_utils.h"
#include <stdio.h>
#include <stdlib.h>

SQLStatement *parseSQLStatment(tokenListCTX *tokenListCTX) {
    // LOGIC:
    // 1) Peek Token.
    // 2) Create space for SQLStatment struct.
    // 3) Decide which path to take.
    // 4) Return struct.
    Token token = peekToken(tokenListCTX);

    SQLStatement *statement = malloc(sizeof(SQLStatement));
    if (!statement) {
        perror("Memory allocation failed for sql statement.");
        free(statement);
        return NULL;
    }

    switch (token.type) {
    case TOKEN_KEYWORD_SELECT:
        statement->type = SELECT_STATEMENT;
        statement->data = parseSelectStatement(tokenListCTX);
        break;
    case TOKEN_KEYWORD_INSERT:
        statement->type = INSERT_STATEMENT;
        statement->data = parseInsertStatement(tokenListCTX);
        break;
    case TOKEN_KEYWORD_UPDATE:
        statement->type = UPDATE_STATEMENT;
        statement->data = parseUpdateStatement(tokenListCTX);
        break;
    case TOKEN_KEYWORD_DELETE:
        statement->type = DELETE_STATEMENT;
        statement->data = parseUpdateStatement(tokenListCTX);
        break;
    case TOKEN_KEYWORD_EXIT:
        statement->type = EXIT_STATEMENT;
        statement->data = parseExitStatement(tokenListCTX);
        break;
    default:
        syntaxError(
            "\nERROR:\n Expected SELECT, INSERT, UPDATE, DELETE, or EXIT");
        free(statement);
        exit(EXIT_FAILURE);
        return NULL;
    }

    return statement;
};
