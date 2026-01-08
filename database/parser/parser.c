#include "parser.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// test
void parse(tokenListCTX *tokenListCTX) {
    // reset position of indexPosition pointer
    tokenListCTX->indexPosition = tokenListCTX->tokenList;

    if (check(0, tokenListCTX->indexPosition->self->type)) {
        printf("\nSelect statement detected!");
        showIndex(tokenListCTX);
        advance(tokenListCTX);
        showIndex(tokenListCTX);
    }
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
