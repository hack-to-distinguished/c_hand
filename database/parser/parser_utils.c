#include "parser_utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

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

void advance(tokenListCTX *tokenListCTX) { tokenListCTX->indexPosition += 1; };

void decrement(tokenListCTX *tokenListCTX) {
    tokenListCTX->indexPosition -= 1;
};

bool check(size_t tokenType, size_t tokenTypeToBeCheckedAgainst) {
    if (tokenType == tokenTypeToBeCheckedAgainst) {
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
                 "\nERROR:\n  Expected: %s\n  But got: %s\n  On line %ld",
                 tokenTypeToString(tokenTypeToBeChecked),
                 tokenTypeToString(tokenType),
                 tokenListCTX->indexPosition->line);

        syntaxError(error_msg);
        exit(EXIT_FAILURE);
        return;
    }
}
