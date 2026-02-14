#include "../parser_utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseUpdateStatement(tokenListCTX *tokenListCTX) {
    ASTNode *updateStatement = malloc(sizeof(ASTNode));

    if (!updateStatement) {
        perror("Memory allocation failed for exit statement.");
        free(updateStatement);
        exit(EXIT_FAILURE);
        return NULL;
    }

    updateStatement->NodeType = AST_UPDATE;
    updateStatement->next = NULL;
    updateStatement->Data.UpdateStatement.whereClause = NULL;
    updateStatement->Data.UpdateStatement.setList = NULL;

    consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_UPDATE,
                 tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_IDENTIFIER,
                 tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_SET,
                 tokenListCTX);

    updateStatement->Data.UpdateStatement.setList = parseSetList(tokenListCTX);

    if (peekToken(tokenListCTX).type == TOKEN_KEYWORD_WHERE) {
        updateStatement->Data.UpdateStatement.whereClause =
            parseWhereClause(tokenListCTX);
    }
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_SEMICOLON,
                 tokenListCTX);

    return updateStatement;
};
