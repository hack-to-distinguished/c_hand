#include "../parser_utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseTableElementList(tokenListCTX *tokenListCTX) {
    ASTNode *tableElementList = malloc(sizeof(ASTNode));

    if (!tableElementList) {
        perror("Memory allocation failed for exit statement.");
        free(tableElementList);
        exit(EXIT_FAILURE);
        return NULL;
    }

    tableElementList->NodeType = AST_TABLE_ELEMENT_LIST;
    tableElementList->next = NULL;
    tableElementList->Data.TableElementList.tableElement =
        parseTableElement(tokenListCTX);

    if (peekToken(tokenListCTX).type == TOKEN_COMMA) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_COMMA,
                     tokenListCTX);
        tableElementList->next = parseTableElementList(tokenListCTX);
    }

    return tableElementList;
};
