#include "../parser_utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseLiteral(tokenListCTX *tokenListCTX) {
    ASTNode *literal = malloc(sizeof(ASTNode));

    if (!literal) {
        perror("Memory allocation failed for exit statement.");
        free(literal);
        exit(EXIT_FAILURE);
        return NULL;
    }

    literal->NodeType = AST_LITERAL;
    literal->next = NULL;

    if (peekToken(tokenListCTX).type == TOKEN_STRING_LITERAL) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_STRING_LITERAL,
                     tokenListCTX);
    } else if (peekToken(tokenListCTX).type == TOKEN_INTEGER_LITERAL) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_INTEGER_LITERAL,
                     tokenListCTX);
    } else {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_FLOAT_LITERAL,
                     tokenListCTX);
    }

    return literal;
};
