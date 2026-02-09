#include "../parser_utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseComparisonOperator(tokenListCTX *tokenListCTX) {
    ASTNode *comparisonOperator = malloc(sizeof(ASTNode));

    if (!comparisonOperator) {
        perror("Memory allocation failed for exit statement.");
        free(comparisonOperator);
        exit(EXIT_FAILURE);
        return NULL;
    }

    comparisonOperator->NodeType = AST_COMPARISON_OPERATOR;
    comparisonOperator->next = NULL;

    if (peekToken(tokenListCTX).type == TOKEN_OPERATOR_EQ) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_OPERATOR_EQ,
                     tokenListCTX);
    } else if (peekToken(tokenListCTX).type == TOKEN_OPERATOR_NEQ) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_OPERATOR_NEQ,
                     tokenListCTX);
    } else if (peekToken(tokenListCTX).type == TOKEN_OPERATOR_LT) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_OPERATOR_LT,
                     tokenListCTX);
    } else if (peekToken(tokenListCTX).type == TOKEN_OPERATOR_LTE) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_OPERATOR_LTE,
                     tokenListCTX);
    } else if (peekToken(tokenListCTX).type == TOKEN_OPERATOR_GT) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_OPERATOR_GT,
                     tokenListCTX);
    } else if (peekToken(tokenListCTX).type == TOKEN_OPERATOR_GTE) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_OPERATOR_GTE,
                     tokenListCTX);
    }

    return comparisonOperator;
};
