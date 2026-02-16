#include "../parser_utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseSetList(tokenListCTX *tokenListCTX) {
    ASTNode *setList = malloc(sizeof(ASTNode));

    if (!setList) {
        perror("Memory allocation failed for exit statement.");
        free(setList);
        exit(EXIT_FAILURE);
        return NULL;
    }

    setList->NodeType = AST_SET_LIST;
    setList->next = NULL;
    setList->Data.SetList.qualifiedIdentifier = NULL;
    setList->Data.SetList.simpleExpression = NULL;

    setList->Data.SetList.qualifiedIdentifier =
        parseQualifiedIdentifier(tokenListCTX);

    consumeToken(tokenListCTX->indexPosition->type, TOKEN_OPERATOR_EQ,
                 tokenListCTX);

    setList->Data.SetList.simpleExpression =
        parseSimpleExpression(tokenListCTX);

    if (peekToken(tokenListCTX).type == TOKEN_COMMA) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_COMMA,
                     tokenListCTX);
        setList->next = parseSetList(tokenListCTX);
    }

    return setList;
};
