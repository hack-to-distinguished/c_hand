#include "../parser_utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseAdditiveOperator(tokenListCTX *tokenListCTX) {
    ASTNode *additiveOperator = malloc(sizeof(ASTNode));

    if (!additiveOperator) {
        perror("Memory allocation failed for exit statement.");
        free(additiveOperator);
        exit(EXIT_FAILURE);
        return NULL;
    }

    additiveOperator->NodeType = AST_ADDITIVE_OPERATOR;
    additiveOperator->next = NULL;
    additiveOperator->Data.AdditiveOperator.term = NULL;

    if (peekToken(tokenListCTX).type == TOKEN_OPERATOR_PLUS) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_OPERATOR_PLUS,
                     tokenListCTX);
    } else {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_OPERATOR_MINUS,
                     tokenListCTX);
    }

    additiveOperator->Data.AdditiveOperator.term = parseTerm(tokenListCTX);

    return additiveOperator;
};
