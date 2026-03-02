#include "../parser_utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseTruncateStatement(tokenListCTX *tokenListCTX) {
    ASTNode *truncateStatement = malloc(sizeof(ASTNode));

    if (!truncateStatement) {
        perror("Memory allocation failed for select list.");
        free(truncateStatement);
        exit(EXIT_FAILURE);
        return NULL;
    }

    truncateStatement->NodeType = AST_TRUNCATE;
    truncateStatement->next = NULL;

    consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_TRUNCATE,
                 tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_TABLE,
                 tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_IDENTIFIER,
                 tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_SEMICOLON,
                 tokenListCTX);

    return truncateStatement;
};
