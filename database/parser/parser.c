#include "parser_utils.h"
#include <stddef.h>
#include <stdlib.h>

void parse(tokenListCTX *tokenListCTX) {
    // reset position of indexPosition pointer
    tokenListCTX->indexPosition = tokenListCTX->tokenList;
    SQLStatement *statement = parseSQLStatment(tokenListCTX);
    if (statement) {
        if (statement->type == EXIT_STATEMENT) {
            if (statement) {
                free(statement->data);
                free(statement);
            }
            exit(1);
        }
        free(statement->data);
        free(statement);
    }
};
