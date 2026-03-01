#include "../parser_utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseAlterAction(tokenListCTX *tokenListCTX) {
    ASTNode *alterAction = malloc(sizeof(ASTNode));

    if (!alterAction) {
        perror("Memory allocation failed for exit statement.");
        free(alterAction);
        exit(EXIT_FAILURE);
        return NULL;
    }

    alterAction->NodeType = AST_ALTER_ACTION;
    alterAction->next = NULL;
    alterAction->Data.AlterAction.alterAddBody = NULL;
    alterAction->Data.AlterAction.alterDropBody = NULL;
    alterAction->Data.AlterAction.columnDefinition = NULL;

    Token nextToken = peekToken(tokenListCTX);

    if (nextToken.type == TOKEN_KEYWORD_ADD) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_ADD,
                     tokenListCTX);
        alterAction->Data.AlterAction.alterAddBody =
            parseAlterAddBody(tokenListCTX);
    } else if (nextToken.type == TOKEN_KEYWORD_DROP) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_DROP,
                     tokenListCTX);
        alterAction->Data.AlterAction.alterDropBody =
            parseAlterDropBody(tokenListCTX);
    } else if (nextToken.type == TOKEN_KEYWORD_MODIFY) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_MODIFY,
                     tokenListCTX);
        alterAction->Data.AlterAction.columnDefinition =
            parseColumnDefinition(tokenListCTX);
    } else if (nextToken.type == TOKEN_KEYWORD_RENAME) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_RENAME,
                     tokenListCTX);
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_COLUMN,
                     tokenListCTX);
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_IDENTIFIER,
                     tokenListCTX);
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_TO,
                     tokenListCTX);
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_IDENTIFIER,
                     tokenListCTX);
    }

    // INFO: do the other types of bodies...

    return alterAction;
};
