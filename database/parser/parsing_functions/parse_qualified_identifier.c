#include "../parser_utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseQualifiedIdentifier(tokenListCTX *tokenListCTX) {
    ASTNode *qualifiedIdentifier = malloc(sizeof(ASTNode));

    if (!qualifiedIdentifier) {
        perror("Memory allocation failed for exit statement.");
        free(qualifiedIdentifier);
        exit(EXIT_FAILURE);
        return NULL;
    }

    qualifiedIdentifier->NodeType = AST_QUALIFIED_IDENTIFIER;
    qualifiedIdentifier->next = NULL;

    consumeToken(tokenListCTX->indexPosition->type, TOKEN_IDENTIFIER,
                 tokenListCTX);

    if (peekToken(tokenListCTX).type == TOKEN_DOT) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_DOT,
                     tokenListCTX);
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_IDENTIFIER,
                     tokenListCTX);
    }

    return qualifiedIdentifier;
};
