#include "parser.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void parse(tokenListCTX *tokenListCTX) {
    // reset position of indexPosition pointer
    tokenListCTX->indexPosition = tokenListCTX->tokenList;
    SQLStatement *statement = parseSQLStatment(tokenListCTX);
    if (statement) {
        free(statement);
    }
};

const char *tokenTypeToString(TokenType s) {
    return (s >= 0) ? tokens[s] : "UNKNOWN";
};

void syntaxError(const char *message) {
    printf("%s", message);
    return;
};

void showIndex(tokenListCTX *tokenListCTX) {
    printf("\nIndex Position: %ld",
           tokenListCTX->indexPosition - tokenListCTX->tokenList);
}

void advance(tokenListCTX *tokenListCTX) {
    // tokenListCTX->indexPosition += 1;
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

Token peekToken(tokenListCTX *tokenListCTX) {
    return *tokenListCTX->indexPosition;
};

void consumeToken(size_t tokenType, size_t tokenTypeToBeChecked,
                  tokenListCTX *tokenListCTX) {
    if (check(tokenType, tokenTypeToBeChecked)) {
        advance(tokenListCTX);
        return;
    } else {
        // Build a detailed error message
        char error_msg[256];
        snprintf(error_msg, sizeof(error_msg),
                 "\nERROR:\n  Expected: %s\n  But got: %s",
                 tokenTypeToString(tokenTypeToBeChecked),
                 tokenTypeToString(tokenType));

        syntaxError(error_msg);
        return;
    }
}

SelectStatement *parseSelectStatement(tokenListCTX *tokenListCTX) {
    return NULL;
};

InsertStatement *parseInsertStatement(tokenListCTX *tokenListCTX) {
    return NULL;
};

UpdateStatement *parseUpdateStatement(tokenListCTX *tokenListCTX) {
    return NULL;
};

DeleteStatement *parseDeleteStatement(tokenListCTX *tokenListCTX) {
    return NULL;
};

ExitStatement *parseExitStatement(tokenListCTX *tokenListCTX) {
    Token token = peekToken(tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_EXIT,
                 tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_SEMICOLON,
                 tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_EOF, tokenListCTX);
    ExitStatement *exitStatement = malloc(sizeof(ExitStatement));

    if (!exitStatement) {
        perror("Memory allocation failed for exit statement.");
        free(exitStatement);
        return NULL;
    }

    return exitStatement;
};

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
        return NULL;
    }

    return statement;
};
