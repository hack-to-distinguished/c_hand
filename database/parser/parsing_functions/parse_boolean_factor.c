#include "../parser_utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseBooleanFactor(tokenListCTX *tokenListCTX) {
    ASTNode *booleanFactor = malloc(sizeof(ASTNode));

    if (!booleanFactor) {
        perror("Memory allocation failed for exit statement.");
        free(booleanFactor);
        exit(EXIT_FAILURE);
        return NULL;
    }

    booleanFactor->NodeType = AST_BOOLEAN_FACTOR;
    booleanFactor->next = NULL;
    booleanFactor->Data.BooleanFactor.comparison = NULL;
    booleanFactor->Data.BooleanFactor.condition = NULL;

    // TODO: add look ahead function for this rule

    return booleanFactor;
};
