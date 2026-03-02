#include "../parser_utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseDropBody(tokenListCTX *tokenListCTX) {
    ASTNode *dropBody = malloc(sizeof(ASTNode));

    if (!dropBody) {
        perror("Memory allocation failed for select list.");
        free(dropBody);
        exit(EXIT_FAILURE);
        return NULL;
    }

    dropBody->NodeType = AST_DROP_BODY;
    dropBody->next = NULL;

    Token nextToken = peekToken(tokenListCTX);

    if (nextToken.type == TOKEN_KEYWORD_TABLE) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_TABLE,
                     tokenListCTX);
        if (peekToken(tokenListCTX).type == TOKEN_KEYWORD_IF) {
            consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_IF,
                         tokenListCTX);
            consumeToken(tokenListCTX->indexPosition->type,
                         TOKEN_KEYWORD_EXISTS, tokenListCTX);
        }
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_IDENTIFIER,
                     tokenListCTX);
    } else if (nextToken.type == TOKEN_KEYWORD_INDEX) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_INDEX,
                     tokenListCTX);
        if (peekToken(tokenListCTX).type == TOKEN_KEYWORD_IF) {
            consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_IF,
                         tokenListCTX);
            consumeToken(tokenListCTX->indexPosition->type,
                         TOKEN_KEYWORD_EXISTS, tokenListCTX);
        }
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_IDENTIFIER,
                     tokenListCTX);
    }

    return dropBody;
};
