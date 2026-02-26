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

    Token nextToken = peekToken(tokenListCTX);

    if ((columnDefinition->Data.ColumnDefinition.columnConstraintList ==
         NULL) &&
        (nextToken.type == TOKEN_KEYWORD_NOT ||
         nextToken.type == TOKEN_KEYWORD_NULL ||
         nextToken.type == TOKEN_KEYWORD_PRIMARY ||
         nextToken.type == TOKEN_KEYWORD_UNIQUE ||
         nextToken.type == TOKEN_KEYWORD_DEFAULT ||
         nextToken.type == TOKEN_KEYWORD_REFERENCES)) {
        columnDefinition->Data.ColumnDefinition.columnConstraintList =
            parseColumnConstraintList(tokenListCTX);
    }

    return columnDefinition;
};
