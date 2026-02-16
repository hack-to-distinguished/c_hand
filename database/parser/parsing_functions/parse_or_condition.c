#include "../parser_utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseOrCondition(tokenListCTX *tokenListCTX) {
    ASTNode *orCondition = malloc(sizeof(ASTNode));

    if (!orCondition) {
        perror("Memory allocation failed for exit statement.");
        free(orCondition);
        exit(EXIT_FAILURE);
        return NULL;
    }

    orCondition->next = NULL;
    orCondition->NodeType = AST_OR_CONDITION;
    orCondition->Data.OrCondition.andCondition = NULL;

    orCondition->Data.OrCondition.andCondition =
        parseAndCondition(tokenListCTX);

    if (peekToken(tokenListCTX).type == TOKEN_KEYWORD_OR) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_OR,
                     tokenListCTX);
        orCondition->next = parseOrCondition(tokenListCTX);
    }

    return orCondition;
};
