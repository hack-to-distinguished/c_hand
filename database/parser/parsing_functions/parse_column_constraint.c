#include "../parser_utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseColumnConstraint(tokenListCTX *tokenListCTX) {
    ASTNode *columnConstraint = malloc(sizeof(ASTNode));

    if (!columnConstraint) {
        perror("Memory allocation failed for exit statement.");
        free(columnConstraint);
        exit(EXIT_FAILURE);
        return NULL;
    }

    columnConstraint->NodeType = AST_COLUMN_CONTRAINT;
    columnConstraint->next = NULL;
    columnConstraint->Data.ColumnConstraint.simpleExpression = NULL;

    Token nextToken = peekToken(tokenListCTX);

    if (nextToken.type == TOKEN_KEYWORD_NOT) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_NOT,
                     tokenListCTX);
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_NULL,
                     tokenListCTX);
    } else if (nextToken.type == TOKEN_KEYWORD_NULL) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_NULL,
                     tokenListCTX);
    } else if (nextToken.type == TOKEN_KEYWORD_PRIMARY) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_PRIMARY,
                     tokenListCTX);
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_KEY,
                     tokenListCTX);
    } else if (nextToken.type == TOKEN_KEYWORD_UNIQUE) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_UNIQUE,
                     tokenListCTX);
    } else if (nextToken.type == TOKEN_KEYWORD_DEFAULT) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_DEFAULT,
                     tokenListCTX);
        columnConstraint->Data.ColumnConstraint.simpleExpression =
            parseSimpleExpression(tokenListCTX);
    } else if (nextToken.type == TOKEN_KEYWORD_REFERENCES) {
        consumeToken(tokenListCTX->indexPosition->type,
                     TOKEN_KEYWORD_REFERENCES, tokenListCTX);
    } else {
        char error_msg[256];
        snprintf(error_msg, sizeof(error_msg),
                 "\nERROR:\n  Expected: %s |\n            %s |\n            %s "
                 "|\n            %s |"
                 "\n            %s |"
                 "\n            %s"
                 "\nBut got: %s\n  "
                 "On line %ld",
                 tokenTypeToString(TOKEN_KEYWORD_NOT),
                 tokenTypeToString(TOKEN_KEYWORD_NULL),
                 tokenTypeToString(TOKEN_KEYWORD_PRIMARY),
                 tokenTypeToString(TOKEN_KEYWORD_UNIQUE),
                 tokenTypeToString(TOKEN_KEYWORD_DEFAULT),
                 tokenTypeToString(TOKEN_KEYWORD_REFERENCES),
                 tokenTypeToString(peekToken(tokenListCTX).type),
                 tokenListCTX->indexPosition->line);

        syntaxError(error_msg);
        exit(EXIT_FAILURE);
        return NULL;
    }

    return columnConstraint;
};
