#include "../parser_utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseCreateTableStatement(tokenListCTX *tokenListCTX) {
    ASTNode *createTableStatement = malloc(sizeof(ASTNode));

    if (!createTableStatement) {
        perror("Memory allocation failed for exit statement.");
        free(createTableStatement);
        exit(EXIT_FAILURE);
        return NULL;
    }

    createTableStatement->NodeType = AST_CREATE_TABLE;
    createTableStatement->Data.CreateTableStatement.tableElementList = NULL;
    createTableStatement->next = NULL;

    return createTableStatement;
};
