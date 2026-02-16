#include "../parser_utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseFactor(tokenListCTX *tokenListCTX) {
    ASTNode *factor = malloc(sizeof(ASTNode));

    if (!factor) {
        perror("Memory allocation failed for exit statement.");
        free(factor);
        exit(EXIT_FAILURE);
        return NULL;
    }

    factor->NodeType = AST_FACTOR;
    factor->next = NULL;
    factor->Data.Factor.simpleExpression = NULL;
    factor->Data.Factor.functionCall = NULL;
    factor->Data.Factor.literal = NULL;
    factor->Data.Factor.qualifiedIdentifier = NULL;

    advance(tokenListCTX);
    Token nextNextToken = peekToken(tokenListCTX);
    decrement(tokenListCTX);
    // INFO: check for qualified identifier first
    if (peekToken(tokenListCTX).type == TOKEN_IDENTIFIER &&
        nextNextToken.type != TOKEN_LPAREN) {
        factor->Data.Factor.qualifiedIdentifier =
            parseQualifiedIdentifier(tokenListCTX);
    } else if (peekToken(tokenListCTX).type == TOKEN_STRING_LITERAL ||
               peekToken(tokenListCTX).type == TOKEN_INTEGER_LITERAL ||
               peekToken(tokenListCTX).type == TOKEN_FLOAT_LITERAL) {
        factor->Data.Factor.literal = parseLiteral(tokenListCTX);
    } else if (peekToken(tokenListCTX).type == TOKEN_KEYWORD_NULL) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_NULL,
                     tokenListCTX);
    } else if (peekToken(tokenListCTX).type == TOKEN_IDENTIFIER &&
               nextNextToken.type == TOKEN_LPAREN) {
        factor->Data.Factor.functionCall = parseFunctionCall(tokenListCTX);
    } else if (peekToken(tokenListCTX).type == TOKEN_LPAREN) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_LPAREN,
                     tokenListCTX);
        factor->Data.Factor.simpleExpression =
            parseSimpleExpression(tokenListCTX);
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_RPAREN,
                     tokenListCTX);

    } else {
        char error_msg[256];
        snprintf(error_msg, sizeof(error_msg),
                 "\nERROR:\n  Expected: %s |\n            %s |\n            %s "
                 "|\n            %s |"
                 "\n            %s |"
                 "\n            %s"
                 "\nBut got: %s\n  "
                 "On line %ld",
                 tokenTypeToString(TOKEN_IDENTIFIER),
                 tokenTypeToString(TOKEN_STRING_LITERAL),
                 tokenTypeToString(TOKEN_INTEGER_LITERAL),
                 tokenTypeToString(TOKEN_FLOAT_LITERAL),
                 tokenTypeToString(TOKEN_KEYWORD_NULL),
                 tokenTypeToString(TOKEN_LPAREN),
                 tokenTypeToString(peekToken(tokenListCTX).type),
                 tokenListCTX->indexPosition->line);

        syntaxError(error_msg);
        exit(EXIT_FAILURE);
        return NULL;
    }

    return factor;
};
