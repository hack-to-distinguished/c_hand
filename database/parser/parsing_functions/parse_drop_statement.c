#include "../parser_utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseDropStatement(tokenListCTX *tokenListCTX) {
    ASTNode *dropStatement = malloc(sizeof(ASTNode));

    if (!dropStatement) {
        perror("Memory allocation failed for select list.");
        free(dropStatement);
        exit(EXIT_FAILURE);
        return NULL;
    }

    dropStatement->NodeType = AST_DROP;
    dropStatement->next = NULL;
    dropStatement->Data.DropStatement.dropBody = NULL;

    consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_DROP,
                 tokenListCTX);
    dropStatement->Data.DropStatement.dropBody = parseDropBody(tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_SEMICOLON,
                 tokenListCTX);

    return dropStatement;
};
