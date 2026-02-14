#include "../parser_utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseOrderClause(tokenListCTX *tokenListCTX) {
    ASTNode *orderList = malloc(sizeof(ASTNode));

    if (!orderList) {
        perror("Memory allocation failed for select list.");
        free(orderList);
        exit(EXIT_FAILURE);
        return NULL;
    }

    orderList->NodeType = AST_ORDER_LIST;
    orderList->next = NULL;
    orderList->Data.OrderList.sortOrder = NULL;
    orderList->Data.OrderList.qualifiedIdentifier = NULL;

    orderList->Data.OrderList.qualifiedIdentifier =
        parseQualifiedIdentifier(tokenListCTX);

    if (peekToken(tokenListCTX).type == TOKEN_KEYWORD_ASC) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_ASC,
                     tokenListCTX);
    } else if (peekToken(tokenListCTX).type == TOKEN_KEYWORD_DESC) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_DESC,
                     tokenListCTX);
    }

    return orderList;
};
