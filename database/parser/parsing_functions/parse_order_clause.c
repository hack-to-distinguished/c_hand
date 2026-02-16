#include "../parser_utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseOrderClause(tokenListCTX *tokenListCTX) {
    ASTNode *orderClause = malloc(sizeof(ASTNode));

    if (!orderClause) {
        perror("Memory allocation failed for select list.");
        free(orderClause);
        exit(EXIT_FAILURE);
        return NULL;
    }

    orderClause->NodeType = AST_ORDER_CLAUSE;
    orderClause->next = NULL;
    orderClause->Data.OrderClause.sortOrder = NULL;
    orderClause->Data.OrderClause.qualifiedIdentifier = NULL;

    orderClause->Data.OrderClause.qualifiedIdentifier =
        parseQualifiedIdentifier(tokenListCTX);

    if (peekToken(tokenListCTX).type == TOKEN_KEYWORD_ASC ||
        peekToken(tokenListCTX).type == TOKEN_KEYWORD_DESC) {
        orderClause->Data.OrderClause.sortOrder = parseSortOrder(tokenListCTX);
    }

    if (peekToken(tokenListCTX).type == TOKEN_COMMA) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_COMMA,
                     tokenListCTX);
        orderClause->next = parseOrderClause(tokenListCTX);
    }

    return orderClause;
};
