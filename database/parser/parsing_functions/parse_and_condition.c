#include "../parser_utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseAndCondition(tokenListCTX *tokenListCTX) {
    ASTNode *andCondition = malloc(sizeof(ASTNode));

    if (!andCondition) {
        perror("Memory allocation failed for exit statement.");
        free(andCondition);
        exit(EXIT_FAILURE);
        return NULL;
    }

    andCondition->NodeType = AST_AND_CONDITION;
    andCondition->next = NULL;
    andCondition->Data.AndCondition.booleanFactor = NULL;

    return andCondition;
};
