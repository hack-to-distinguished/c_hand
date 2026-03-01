#include "../parser_utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseAlterStatement(tokenListCTX *tokenListCTX) {
    ASTNode *alterStatement = malloc(sizeof(ASTNode));

    if (!alterStatement) {
        perror("Memory allocation failed for exit statement.");
        free(alterStatement);
        exit(EXIT_FAILURE);
        return NULL;
    }

    alterStatement->NodeType = AST_ALTER;
    alterStatement->next = NULL;
    alterStatement->Data.AlterStatement.alterAction = NULL;

    consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_ALTER,
                 tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_TABLE,
                 tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_IDENTIFIER,
                 tokenListCTX);
    alterStatement->Data.AlterStatement.alterAction =
        parseAlterAction(tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_SEMICOLON,
                 tokenListCTX);
    return alterStatement;
};
