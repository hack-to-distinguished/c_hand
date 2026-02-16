#include "../parser_utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseExpression(tokenListCTX *tokenListCTX) {
    ASTNode *expression = malloc(sizeof(ASTNode));

    if (!expression) {
        perror("Memory allocation failed for exit statement.");
        free(expression);
        exit(EXIT_FAILURE);
        return NULL;
    }

    expression->NodeType = AST_EXPRESSION;
    expression->next = NULL;
    expression->Data.Expression.simpleExpressionL = NULL;
    expression->Data.Expression.simpleExpressionR = NULL;
    expression->Data.Expression.comparisonOperator = NULL;

    expression->Data.Expression.simpleExpressionL =
        parseSimpleExpression(tokenListCTX);
    expression->Data.Expression.comparisonOperator =
        parseComparisonOperator(tokenListCTX);
    expression->Data.Expression.simpleExpressionR =
        parseSimpleExpression(tokenListCTX);

    return expression;
};
