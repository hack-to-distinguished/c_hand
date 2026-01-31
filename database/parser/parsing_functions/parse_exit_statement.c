#include "../parser_utils.h"
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseExitStatement(tokenListCTX *tokenListCTX) {
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_EXIT,
                 tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_SEMICOLON,
                 tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_EOF, tokenListCTX);
    ASTNode *exitStatement = malloc(sizeof(ASTNode));
    exitStatement->NodeType = AST_EXIT;
    exitStatement->next = NULL;

    if (!exitStatement) {
        perror("Memory allocation failed for exit statement.");
        free(exitStatement);
        exit(EXIT_FAILURE);
        return NULL;
    }

    return exitStatement;
};
