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
    functionCall->Data.FunctionCall.argList = NULL;

    consumeToken(tokenListCTX->indexPosition->type, TOKEN_IDENTIFIER,
                 tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_LPAREN, tokenListCTX);
    Token nextToken = peekToken(tokenListCTX);
    if (nextToken.type != TOKEN_RPAREN) {
        ASTNode *argList = parseArgList(tokenListCTX);
        functionCall->Data.FunctionCall.argList = argList;
    }
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_RPAREN, tokenListCTX);

    return functionCall;
};
