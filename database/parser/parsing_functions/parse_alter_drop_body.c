#include "../parser_utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseAlterDropBody(tokenListCTX *tokenListCTX) {
    ASTNode *alterDropBody = malloc(sizeof(ASTNode));

    if (!alterDropBody) {
        perror("Memory allocation failed for exit statement.");
        free(alterDropBody);
        exit(EXIT_FAILURE);
        return NULL;
    }

    alterDropBody->NodeType = AST_ALTER_DROP_BODY;
    alterDropBody->next = NULL;

    Token nextToken = peekToken(tokenListCTX);

    if (nextToken.type == TOKEN_KEYWORD_COLUMN) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_COLUMN,
                     tokenListCTX);
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_IDENTIFIER,
                     tokenListCTX);
    } else if (nextToken.type == TOKEN_KEYWORD_CONSTRAINT) {
        consumeToken(tokenListCTX->indexPosition->type,
                     TOKEN_KEYWORD_CONSTRAINT, tokenListCTX);
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_IDENTIFIER,
                     tokenListCTX);
    }

    return alterDropBody;
};
