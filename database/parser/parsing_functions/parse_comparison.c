#include "../parser_utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseComparison(tokenListCTX *tokenListCTX) {
    ASTNode *comparison = malloc(sizeof(ASTNode));

    if (!comparison) {
        perror("Memory allocation failed for exit statement.");
        free(comparison);
        exit(EXIT_FAILURE);
        return NULL;
    }

    comparison->NodeType = AST_COMPARISON;
    comparison->next = NULL;
    comparison->Data.Comparison.simpleExpressionL = NULL;
    comparison->Data.Comparison.comparisonOperator = NULL;
    comparison->Data.Comparison.simpleExpressionR = NULL;

    comparison->Data.Comparison.simpleExpressionL =
        parseSimpleExpression(tokenListCTX);
    comparison->Data.Comparison.comparisonOperator =
        parseComparisonOperator(tokenListCTX);
    comparison->Data.Comparison.simpleExpressionR =
        parseSimpleExpression(tokenListCTX);
    return comparison;
};
