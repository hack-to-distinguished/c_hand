#include "../parser_utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseArgList(tokenListCTX *tokenListCTX) {
    ASTNode *argList = malloc(sizeof(ASTNode));

    if (!argList) {
        perror("Memory allocation failed for exit statement.");
        free(argList);
        exit(EXIT_FAILURE);
        return NULL;
    }

    argList->next = NULL;
    argList->NodeType = AST_ARG_LIST;
    argList->Data.ArgList.simpleExpression = NULL;

    argList->Data.ArgList.simpleExpression =
        parseSimpleExpression(tokenListCTX);

    if (peekToken(tokenListCTX).type == TOKEN_COMMA) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_COMMA,
                     tokenListCTX);
        argList->next = parseArgList(tokenListCTX);
    }

    return argList;
};
