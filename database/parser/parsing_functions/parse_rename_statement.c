#include "../parser_utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseRenameStatement(tokenListCTX *tokenListCTX) {
    ASTNode *renameStatement = malloc(sizeof(ASTNode));

    if (!renameStatement) {
        perror("Memory allocation failed for select list.");
        free(renameStatement);
        exit(EXIT_FAILURE);
        return NULL;
    }

    renameStatement->NodeType = AST_RENAME;
    renameStatement->next = NULL;

    consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_RENAME,
                 tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_TABLE,
                 tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_IDENTIFIER,
                 tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_TO,
                 tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_IDENTIFIER,
                 tokenListCTX);
    return renameStatement;
};
