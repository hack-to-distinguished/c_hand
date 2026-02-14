#include "../parser_utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseSelectList(tokenListCTX *tokenListCTX) {

    ASTNode *selectList = malloc(sizeof(ASTNode));

    if (!selectList) {
        perror("Memory allocation failed for select list.");
        free(selectList);
        exit(EXIT_FAILURE);
        return NULL;
    }

    selectList->NodeType = AST_SELECT_LIST;
    selectList->next = NULL;
    selectList->Data.SelectList.simpleExpression = NULL;
    selectList->Data.SelectList.simpleExpression =
        parseSimpleExpression(tokenListCTX);

    if (peekToken(tokenListCTX).type == TOKEN_COMMA) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_COMMA,
                     tokenListCTX);
        selectList->next = parseSelectList(tokenListCTX);
    }

    return selectList;
};
