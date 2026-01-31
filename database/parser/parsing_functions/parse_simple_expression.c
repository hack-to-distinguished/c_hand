#include "../parser_utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseSimpleExpression(tokenListCTX *tokenListCTX) {
    ASTNode *simpleExpression = malloc(sizeof(ASTNode));

    if (!simpleExpression) {
        perror("Memory allocation failed for exit statement.");
        free(simpleExpression);
        exit(EXIT_FAILURE);
        return NULL;
    }

    simpleExpression->NodeType = AST_SIMPLE_EXPRESSION;
    simpleExpression->Data.SimpleExpression.term = NULL;
    simpleExpression->Data.SimpleExpression.additiveOperator = NULL;
    simpleExpression->Data.SimpleExpression.term = parseTerm(tokenListCTX);

    ASTNode *tail = NULL;

    while (peekToken(tokenListCTX).type == TOKEN_OPERATOR_PLUS ||
           peekToken(tokenListCTX).type == TOKEN_OPERATOR_MINUS) {
        ASTNode *newAdditiveOperator = parseAdditiveOperator(tokenListCTX);

        // INFO: first additive op
        if (simpleExpression->Data.SimpleExpression.additiveOperator == NULL) {
            simpleExpression->Data.SimpleExpression.additiveOperator =
                newAdditiveOperator;
            tail = newAdditiveOperator;
        } else {
            tail->next = newAdditiveOperator;
            tail = tail->next;
        }
    }

    return simpleExpression;
};
