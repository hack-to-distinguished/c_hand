#include "../parser_utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseMultiplicativeOperator(tokenListCTX *tokenListCTX) {
    ASTNode *multiplicativeOperator = malloc(sizeof(ASTNode));

    if (!multiplicativeOperator) {
        perror("Memory allocation failed for exit statement.");
        free(multiplicativeOperator);
        exit(EXIT_FAILURE);
        return NULL;
    }

    multiplicativeOperator->NodeType = AST_MULTIPLICATIVE_OPERATOR;
    multiplicativeOperator->next = NULL;
    multiplicativeOperator->Data.MultiplicativeOperator.factor = NULL;

    if (peekToken(tokenListCTX).type == TOKEN_OPERATOR_STAR) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_OPERATOR_STAR,
                     tokenListCTX);
    } else {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_OPERATOR_SLASH,
                     tokenListCTX);
    }

    multiplicativeOperator->Data.MultiplicativeOperator.factor =
        parseFactor(tokenListCTX);

    return multiplicativeOperator;
};
