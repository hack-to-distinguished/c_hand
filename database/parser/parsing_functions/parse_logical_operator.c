#include "../parser_utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseLogicalOperator(tokenListCTX *tokenListCTX) {
    ASTNode *logicalOperator = malloc(sizeof(ASTNode));

    if (!logicalOperator) {
        perror("Memory allocation failed for exit statement.");
        free(logicalOperator);
        exit(EXIT_FAILURE);
        return NULL;
    }

    logicalOperator->NodeType = AST_LOGICAL_OPERATOR;
    logicalOperator->Data.LogicalOperator.expression = NULL;
    logicalOperator->next = NULL;

    if (peekToken(tokenListCTX).type == TOKEN_KEYWORD_AND) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_AND,
                     tokenListCTX);
    } else if (peekToken(tokenListCTX).type == TOKEN_KEYWORD_OR) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_OR,
                     tokenListCTX);
    }

    logicalOperator->Data.LogicalOperator.expression =
        parseExpression(tokenListCTX);
    return logicalOperator;
};
