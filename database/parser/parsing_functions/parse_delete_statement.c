#include "../parser_utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseDeleteStatement(tokenListCTX *tokenListCTX) {
    ASTNode *deleteStatement = malloc(sizeof(ASTNode));

    if (!deleteStatement) {
        perror("Memory allocation failed for exit statement.");
        free(deleteStatement);
        exit(EXIT_FAILURE);
        return NULL;
    }

    deleteStatement->NodeType = AST_DELETE;
    deleteStatement->next = NULL;
    deleteStatement->Data.DeleteStatement.whereClause = NULL;

    consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_DELETE,
                 tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_FROM,
                 tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_IDENTIFIER,
                 tokenListCTX);

    if (peekToken(tokenListCTX).type == TOKEN_KEYWORD_WHERE) {
        deleteStatement->Data.DeleteStatement.whereClause =
            parseWhereClause(tokenListCTX);
    }

    consumeToken(tokenListCTX->indexPosition->type, TOKEN_SEMICOLON,
                 tokenListCTX);
    return deleteStatement;
};
