#include "../parser_utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseCondition(tokenListCTX *tokenListCTX) {
    ASTNode *condition = malloc(sizeof(ASTNode));

    if (!condition) {
        perror("Memory allocation failed for exit statement.");
        free(condition);
        exit(EXIT_FAILURE);
        return NULL;
    }

    condition->NodeType = AST_CONDITION;
    condition->Data.Condition.orCondition = NULL;
    condition->next = NULL;

    condition->Data.Condition.orCondition = parseOrCondition(tokenListCTX);

    return condition;
};
