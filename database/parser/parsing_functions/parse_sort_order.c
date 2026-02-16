#include "../parser_utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseSortOrder(tokenListCTX *tokenListCTX) {

    ASTNode *sortOrder = malloc(sizeof(ASTNode));

    if (!sortOrder) {
        perror("Memory allocation failed for select list.");
        free(sortOrder);
        exit(EXIT_FAILURE);
        return NULL;
    }

    sortOrder->NodeType = AST_SORT_ORDER;
    sortOrder->next = NULL;

    if (peekToken(tokenListCTX).type == TOKEN_KEYWORD_ASC) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_ASC,
                     tokenListCTX);
    } else if (peekToken(tokenListCTX).type == TOKEN_KEYWORD_DESC) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_DESC,
                     tokenListCTX);
    }
    return sortOrder;
};
