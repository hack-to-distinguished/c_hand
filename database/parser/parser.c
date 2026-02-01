#include "parser_utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void parse(tokenListCTX *tokenListCTX) {
    // reset position of indexPosition pointer

    tokenListCTX->indexPosition = tokenListCTX->tokenList;
    ASTNode *sqlStatement = parseSQLStatment(tokenListCTX);
    if (sqlStatement) {
        if (sqlStatement->NodeType == AST_EXIT) {
            printf("\nEXITING...");
            free(sqlStatement);
            exit(EXIT_SUCCESS);
        }
        // TODO: free AST
        free(sqlStatement);
    }
};
