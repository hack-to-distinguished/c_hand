#include "../parser_utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseCreateStatement(tokenListCTX *tokenListCTX) {
    ASTNode *createStatement = malloc(sizeof(ASTNode));

    if (!createStatement) {
        perror("Memory allocation failed for exit statement.");
        free(createStatement);
        exit(EXIT_FAILURE);
        return NULL;
    }

    createStatement->NodeType = AST_CREATE;
    createStatement->next = NULL;
    createStatement->Data.CreateStatement.createBody = NULL;

    consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_CREATE,
                 tokenListCTX);

    createStatement->Data.CreateStatement.createBody =
        parseCreateBody(tokenListCTX);

    return createStatement;
};
