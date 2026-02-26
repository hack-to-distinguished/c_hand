#include "../parser_utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseColumnConstraintList(tokenListCTX *tokenListCTX) {
    ASTNode *columnConstraintList = malloc(sizeof(ASTNode));

    if (!columnConstraintList) {
        perror("Memory allocation failed for exit statement.");
        free(columnConstraintList);
        exit(EXIT_FAILURE);
        return NULL;
    }

    columnConstraintList->NodeType = AST_COLUMN_CONTRAINT_LIST;
    columnConstraintList->next = NULL;
    columnConstraintList->Data.ColumnConstraintList.columnConstraint = NULL;

    columnConstraintList->Data.ColumnConstraintList.columnConstraint =
        parseColumnConstraint(tokenListCTX);

    Token nextToken = peekToken(tokenListCTX);

    if (nextToken.type == TOKEN_KEYWORD_NOT ||
        nextToken.type == TOKEN_KEYWORD_NULL ||
        nextToken.type == TOKEN_KEYWORD_PRIMARY ||
        nextToken.type == TOKEN_KEYWORD_UNIQUE ||
        nextToken.type == TOKEN_KEYWORD_DEFAULT ||
        nextToken.type == TOKEN_KEYWORD_REFERENCES) {
        columnConstraintList->next = parseColumnConstraintList(tokenListCTX);
    }

    return columnConstraintList;
};
