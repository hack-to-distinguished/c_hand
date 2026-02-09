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
    condition->next = NULL;
    condition->Data.Condition.expression = NULL;
    condition->Data.Condition.logicalOperator = NULL;

    condition->Data.Condition.expression = parseExpression(tokenListCTX);

    ASTNode *tail = NULL;

    while (peekToken(tokenListCTX).type == TOKEN_KEYWORD_AND ||
           peekToken(tokenListCTX).type == TOKEN_KEYWORD_OR) {
        ASTNode *newLogicalOperator = parseLogicalOperator(tokenListCTX);

        if (condition->Data.Condition.logicalOperator == NULL) {
            condition->Data.Condition.logicalOperator = newLogicalOperator;
            tail = newLogicalOperator;
        } else {
            tail->next = newLogicalOperator;
            tail = tail->next;
        }
    }

    return condition;
};
