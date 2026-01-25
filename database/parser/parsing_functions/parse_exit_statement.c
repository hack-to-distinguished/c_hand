#include "../parser_utils.h"
#include <stdio.h>
#include <stdlib.h>

ExitStatement *parseExitStatement(tokenListCTX *tokenListCTX) {
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_EXIT,
                 tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_SEMICOLON,
                 tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_EOF, tokenListCTX);
    ExitStatement *exitStatement = malloc(sizeof(ExitStatement));

    if (!exitStatement) {
        perror("Memory allocation failed for exit statement.");
        free(exitStatement);
        exit(EXIT_FAILURE);
        return NULL;
    }

    return exitStatement;
};
