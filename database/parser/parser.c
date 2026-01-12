#include "parser.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void parse(tokenListCTX *tokenListCTX) {
    // reset position of indexPosition pointer
    tokenListCTX->indexPosition = tokenListCTX->tokenList;
    SQLStatement *statement = parseSQLStatment(tokenListCTX);
    free(statement);
};

void showIndex(tokenListCTX *tokenListCTX) {
    printf("\nIndex Position: %ld",
           tokenListCTX->indexPosition - tokenListCTX->tokenList);
}

void advance(tokenListCTX *tokenListCTX) {
    if ((tokenListCTX->tokenList - tokenListCTX->indexPosition + 1) <
        tokenListCTX->currentSize) {
        tokenListCTX->indexPosition += 1;
    }
};

bool check(size_t tokenType, size_t tokenTypeToBeChecked) {
    if (tokenType == tokenTypeToBeChecked) {
        return true;
    }
    return false;
};

Token *peekToken(tokenListCTX *tokenListCTX) {
    return tokenListCTX->indexPosition;
};

SelectStatement *parseSelectStatement() { return NULL; };

InsertStatement *parseInsertStatement() { return NULL; };

UpdateStatement *parseUpdateStatement() { return NULL; };

DeleteStatement *parseDeleteStatement() { return NULL; };

ExitStatement *parseExitStatement() { return NULL; };

SQLStatement *parseSQLStatment(tokenListCTX *tokenListCTX) {
    // LOGIC:
    // 1) Peek Token.
    // 2) Create space for SQLStatment struct.
    // 3) Decide which path to take.
    // 4) Return struct.
    Token *token = peekToken(tokenListCTX);

    SQLStatement *statement = malloc(sizeof(SQLStatement));
    if (!statement) {
        perror("Memory allocation failed for sql statement.");
        free(statement);
        return NULL;
    }

    switch (token->type) {
    case TOKEN_KEYWORD_SELECT:
        printf("\nSELECT STATEMENT DETECTED");
        statement->type = SELECT_STATEMENT;
        statement->data = parseSelectStatement();
        break;
    case TOKEN_KEYWORD_INSERT:
        printf("\nINSERT STATEMENT DETECTED");
        statement->type = INSERT_STATEMENT;
        statement->data = parseInsertStatement();
        break;
    case TOKEN_KEYWORD_UPDATE:
        printf("\nUPDATE STATEMENT DETECTED");
        statement->type = UPDATE_STATEMENT;
        statement->data = parseUpdateStatement();
        break;
    case TOKEN_KEYWORD_DELETE:
        printf("\nDELETE STATEMENT DETECTED");
        statement->type = DELETE_STATEMENT;
        statement->data = parseUpdateStatement();
        break;
    case TOKEN_KEYWORD_EXIT:
        printf("\nEXIT STATEMENT DETECTED");
        statement->type = EXIT_STATEMENT;
        statement->data = parseExitStatement();
        break;
    default:
        perror("Expected SELECT, INSERT, UPDATE, DELETE, or EXIT");
        free(statement);
        return NULL;
    }

    return statement;
};
