#include "parser_utils.h"
#include <stdio.h>
#include <stdlib.h>

SelectStatement *parseSelectStatement(tokenListCTX *tokenListCTX) {
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_SELECT,
                 tokenListCTX);

    Token nextToken = peekToken(tokenListCTX);
    if (nextToken.type == TOKEN_OPERATOR_STAR) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_OPERATOR_STAR,
                     tokenListCTX);
    } else if (nextToken.type == TOKEN_IDENTIFIER) {
        // TODO: Create select list parser
    }

    consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_FROM,
                 tokenListCTX);

    // TODO: OPTIONAL statements:
    // TODO: Create where clause parser
    // TODO: Create order by clause parser

    consumeToken(tokenListCTX->indexPosition->type, TOKEN_SEMICOLON,
                 tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_EOF, tokenListCTX);

    SelectStatement *selectStatement = malloc(sizeof(SelectStatement));

    if (!selectStatement) {
        perror("Memory allocation failed for exit statement.");
        free(selectStatement);
        exit(EXIT_FAILURE);
        return NULL;
    }

    // TODO: If user inputted extra statements, insert corresponding nodes into
    // select statement tree

    return selectStatement;
};
