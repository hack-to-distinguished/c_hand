#include "../parser_utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseTableList(tokenListCTX *tokenListCTX) {

    ASTNode *tableList = malloc(sizeof(ASTNode));

    if (!tableList) {
        perror("Memory allocation failed for select list.");
        free(tableList);
        exit(EXIT_FAILURE);
        return NULL;
    }

    tableList->next = NULL;
    tableList->NodeType = AST_TABLE_LIST;

    consumeToken(tokenListCTX->indexPosition->type, TOKEN_IDENTIFIER,
                 tokenListCTX);

    while (peekToken(tokenListCTX).type == TOKEN_COMMA) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_COMMA,
                     tokenListCTX);

        consumeToken(tokenListCTX->indexPosition->type, TOKEN_IDENTIFIER,
                     tokenListCTX);
    }

    return tableList;
};
