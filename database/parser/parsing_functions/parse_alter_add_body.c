#include "../parser_utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseAlterAddBody(tokenListCTX *tokenListCTX) {
    ASTNode *alterAddBody = malloc(sizeof(ASTNode));

    if (!alterAddBody) {
        perror("Memory allocation failed for exit statement.");
        free(alterAddBody);
        exit(EXIT_FAILURE);
        return NULL;
    }

    alterAddBody->NodeType = AST_ALTER_ADD_BODY;
    alterAddBody->next = NULL;
    alterAddBody->Data.AlterAddBody.columnDefinition = NULL;
    alterAddBody->Data.AlterAddBody.tableConstraint = NULL;

    if (peekToken(tokenListCTX).type == TOKEN_KEYWORD_COLUMN) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_COLUMN,
                     tokenListCTX);
        alterAddBody->Data.AlterAddBody.columnDefinition =
            parseColumnDefinition(tokenListCTX);
    } else {
        alterAddBody->Data.AlterAddBody.tableConstraint =
            parseTableConstraint(tokenListCTX);
    }

    return alterAddBody;
};
