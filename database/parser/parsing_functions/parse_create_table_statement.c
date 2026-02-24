#include "../parser_utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseCreateTableStatement(tokenListCTX *tokenListCTX) {
    ASTNode *createTableStatement = malloc(sizeof(ASTNode));

    if (!createTableStatement) {
        perror("Memory allocation failed for exit statement.");
        free(createTableStatement);
        exit(EXIT_FAILURE);
        return NULL;
    }

    createTableStatement->NodeType = AST_CREATE_TABLE;
    createTableStatement->Data.CreateTableStatement.tableElementList = NULL;
    createTableStatement->next = NULL;

    consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_TABLE,
                 tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_IDENTIFIER,
                 tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_LPAREN, tokenListCTX);
    createTableStatement->Data.CreateTableStatement.tableElementList =
        parseTableElementList(tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_RPAREN, tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_SEMICOLON,
                 tokenListCTX);

    return createTableStatement;
};
