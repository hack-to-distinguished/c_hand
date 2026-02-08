#include "../parser_utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseFunctionCall(tokenListCTX *tokenListCTX) {
    ASTNode *functionCall = malloc(sizeof(ASTNode));

    if (!functionCall) {
        perror("Memory allocation failed for exit statement.");
        free(functionCall);
        exit(EXIT_FAILURE);
        return NULL;
    }

    functionCall->NodeType = AST_FUNCTION_CALL;
    functionCall->next = NULL;

    consumeToken(tokenListCTX->indexPosition->type, TOKEN_IDENTIFIER,
                 tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_LPAREN, tokenListCTX);
    // TODO: 0 or 1 arg list...
    ASTNode *argList = parseArgList(tokenListCTX);
    if (argList) {
        functionCall->Data.FunctionCall.argList = argList;
    }
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_RPAREN, tokenListCTX);

    return functionCall;
};
