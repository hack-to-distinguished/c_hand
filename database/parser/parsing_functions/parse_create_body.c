#include "../parser_utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseCreateBody(tokenListCTX *tokenListCTX) {
    ASTNode *createBody = malloc(sizeof(ASTNode));

    if (!createBody) {
        perror("Memory allocation failed for exit statement.");
        free(createBody);
        exit(EXIT_FAILURE);
        return NULL;
    }

    createBody->NodeType = AST_CREATE_BODY;
    createBody->Data.CreateBody.createIndexStatement = NULL;
    createBody->Data.CreateBody.createTableStatement = NULL;
    createBody->next = NULL;

    if (peekToken(tokenListCTX).type == TOKEN_KEYWORD_TABLE) {
        createBody->Data.CreateBody.createTableStatement =
            parseCreateTableStatement(tokenListCTX);
    } else if (peekToken(tokenListCTX).type == TOKEN_KEYWORD_UNIQUE ||
               peekToken(tokenListCTX).type == TOKEN_KEYWORD_INDEX) {
        createBody->Data.CreateBody.createIndexStatement =
            parseCreateIndexStatement(tokenListCTX);
    }

    return createBody;
};
