#include "../parser_utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseAndCondition(tokenListCTX *tokenListCTX) {
    ASTNode *andCondition = malloc(sizeof(ASTNode));

    if (!andCondition) {
        perror("Memory allocation failed for exit statement.");
        free(andCondition);
        exit(EXIT_FAILURE);
        return NULL;
    }

    andCondition->NodeType = AST_AND_CONDITION;
    andCondition->next = NULL;
    andCondition->Data.AndCondition.booleanFactor = NULL;

    andCondition->Data.AndCondition.booleanFactor =
        parseBooleanFactor(tokenListCTX);

    if (peekToken(tokenListCTX).type == TOKEN_KEYWORD_AND) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_AND,
                     tokenListCTX);
        andCondition->next = parseAndCondition(tokenListCTX);
    }

    return andCondition;
};
