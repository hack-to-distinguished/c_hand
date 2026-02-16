#include "../parser_utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseValueList(tokenListCTX *tokenListCTX) {
    ASTNode *valueList = malloc(sizeof(ASTNode));

    if (!valueList) {
        perror("Memory allocation failed for select list.");
        free(valueList);
        exit(EXIT_FAILURE);
        return NULL;
    }

    valueList->NodeType = AST_VALUE_LIST;
    valueList->next = NULL;
    valueList->Data.ValueList.simpleExpression = NULL;

    valueList->Data.ValueList.simpleExpression =
        parseSimpleExpression(tokenListCTX);

    if (peekToken(tokenListCTX).type == TOKEN_COMMA) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_COMMA,
                     tokenListCTX);
        valueList->next = parseValueList(tokenListCTX);
    }

    return valueList;
};
