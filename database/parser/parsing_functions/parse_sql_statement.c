#include "../parser_utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseSQLStatment(tokenListCTX *tokenListCTX) {
    Token token = peekToken(tokenListCTX);

    switch (token.type) {
    case TOKEN_KEYWORD_SELECT:
        return parseSelectStatement(tokenListCTX);
        break;
    case TOKEN_KEYWORD_INSERT:
        return parseInsertStatement(tokenListCTX);
        break;
    case TOKEN_KEYWORD_UPDATE:
        return parseUpdateStatement(tokenListCTX);
        break;
    case TOKEN_KEYWORD_DELETE:
        return parseDeleteStatement(tokenListCTX);
        break;
    case TOKEN_KEYWORD_EXIT:
        return parseExitStatement(tokenListCTX);
        break;
    default: {
        char error_msg[256];
        snprintf(error_msg, sizeof(error_msg),
                 "\nERROR:\n  Expected SELECT, INSERT, UPDATE, DELETE, or "
                 "EXIT\n  On line %ld",
                 tokenListCTX->indexPosition->line);
        syntaxError(error_msg);
        exit(EXIT_FAILURE);
        return NULL;
    }
    }
    return NULL;
};
