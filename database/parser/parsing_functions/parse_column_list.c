#include "../parser_utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseColumnList(tokenListCTX *tokenListCTX) {
    ASTNode *columnList = malloc(sizeof(ASTNode));

    if (!columnList) {
        perror("Memory allocation failed for select list.");
        free(columnList);
        exit(EXIT_FAILURE);
        return NULL;
    }

    columnList->NodeType = AST_COLUMN_LIST;
    columnList->next = NULL;
    columnList->Data.ColumnList.qualifiedIdentifier = NULL;

    columnList->Data.ColumnList.qualifiedIdentifier =
        parseQualifiedIdentifier(tokenListCTX);

    if (peekToken(tokenListCTX).type == TOKEN_COMMA) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_COMMA,
                     tokenListCTX);
        columnList->next = parseColumnList(tokenListCTX);
    }

    return columnList;
};
