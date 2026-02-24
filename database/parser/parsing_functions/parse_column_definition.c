#include "../parser_utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseColumnDefinition(tokenListCTX *tokenListCTX) {
    ASTNode *columnDefinition = malloc(sizeof(ASTNode));

    if (!columnDefinition) {
        perror("Memory allocation failed for exit statement.");
        free(columnDefinition);
        exit(EXIT_FAILURE);
        return NULL;
    }

    columnDefinition->NodeType = AST_COLUMN_DEFINITION;
    columnDefinition->Data.ColumnDefinition.dataType = NULL;
    columnDefinition->Data.ColumnDefinition.columnConstraintList = NULL;
    columnDefinition->next = NULL;

    consumeToken(tokenListCTX->indexPosition->type, TOKEN_IDENTIFIER,
                 tokenListCTX);
    columnDefinition->Data.ColumnDefinition.dataType =
        parseDataType(tokenListCTX);

    // INFO: Do 0 or 1 of 'column_constraint_list'

    return columnDefinition;
};
