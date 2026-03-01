#include "../parser_utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseCreateIndexStatement(tokenListCTX *tokenListCTX) {
    ASTNode *createIndexStatement = malloc(sizeof(ASTNode));

    if (!createIndexStatement) {
        perror("Memory allocation failed for exit statement.");
        free(createIndexStatement);
        exit(EXIT_FAILURE);
        return NULL;
    }

    createIndexStatement->NodeType = AST_CREATE_INDEX;
    createIndexStatement->Data.CreateIndexStatement.columnList = NULL;
    createIndexStatement->next = NULL;

    Token nextToken = peekToken(tokenListCTX);

    if (nextToken.type == TOKEN_KEYWORD_UNIQUE) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_UNIQUE,
                     tokenListCTX);
    }

    consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_INDEX,
                 tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_IDENTIFIER,
                 tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_ON,
                 tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_IDENTIFIER,
                 tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_LPAREN, tokenListCTX);
    createIndexStatement->Data.CreateIndexStatement.columnList =
        parseColumnList(tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_RPAREN, tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_SEMICOLON,
                 tokenListCTX);

    return createIndexStatement;
};
