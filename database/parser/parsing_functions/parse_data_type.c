#include "../parser_utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseDataType(tokenListCTX *tokenListCTX) {
    ASTNode *dataType = malloc(sizeof(ASTNode));

    if (!dataType) {
        perror("Memory allocation failed for exit statement.");
        free(dataType);
        exit(EXIT_FAILURE);
        return NULL;
    }

    dataType->NodeType = AST_DATA_TYPE;
    dataType->next = NULL;

    Token nextToken = peekToken(tokenListCTX);

    if (nextToken.type == TOKEN_KEYWORD_INT) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_INT,
                     tokenListCTX);
    } else if (nextToken.type == TOKEN_KEYWORD_INTEGER) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_INTEGER,
                     tokenListCTX);
    } else if (nextToken.type == TOKEN_KEYWORD_FLOAT) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_FLOAT,
                     tokenListCTX);
    } else if (nextToken.type == TOKEN_KEYWORD_DOUBLE) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_DOUBLE,
                     tokenListCTX);
    } else if (nextToken.type == TOKEN_KEYWORD_BOOLEAN) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_BOOLEAN,
                     tokenListCTX);
    } else if (nextToken.type == TOKEN_KEYWORD_DATE) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_DATE,
                     tokenListCTX);
    } else if (nextToken.type == TOKEN_KEYWORD_TEXT) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_TEXT,
                     tokenListCTX);
    } else if (nextToken.type == TOKEN_KEYWORD_VARCHAR) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_VARCHAR,
                     tokenListCTX);
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_LPAREN,
                     tokenListCTX);
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_INTEGER_LITERAL,
                     tokenListCTX);
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_RPAREN,
                     tokenListCTX);
    } else if (nextToken.type == TOKEN_KEYWORD_CHAR) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_CHAR,
                     tokenListCTX);
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_LPAREN,
                     tokenListCTX);
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_INTEGER_LITERAL,
                     tokenListCTX);
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_RPAREN,
                     tokenListCTX);
    } else if (nextToken.type == TOKEN_KEYWORD_DECIMAL) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_DECIMAL,
                     tokenListCTX);
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_LPAREN,
                     tokenListCTX);
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_INTEGER_LITERAL,
                     tokenListCTX);
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_COMMA,
                     tokenListCTX);
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_INTEGER_LITERAL,
                     tokenListCTX);
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_RPAREN,
                     tokenListCTX);
    }

    return dataType;
};
