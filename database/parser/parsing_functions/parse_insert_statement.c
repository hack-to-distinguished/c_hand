#include "../parser_utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseInsertStatement(tokenListCTX *tokenListCTX) {
    ASTNode *insertStatement = malloc(sizeof(ASTNode));

    if (!insertStatement) {
        perror("Memory allocation failed for exit statement.");
        free(insertStatement);
        exit(EXIT_FAILURE);
        return NULL;
    }

    insertStatement->NodeType = AST_INSERT;
    insertStatement->next = NULL;
    insertStatement->Data.InsertStatement.columnList = NULL;
    insertStatement->Data.InsertStatement.valueList = NULL;

    consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_INSERT,
                 tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_INTO,
                 tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_IDENTIFIER,
                 tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_LPAREN, tokenListCTX);

    insertStatement->Data.InsertStatement.columnList =
        parseColumnList(tokenListCTX);

    consumeToken(tokenListCTX->indexPosition->type, TOKEN_RPAREN, tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_VALUES,
                 tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_LPAREN, tokenListCTX);

    insertStatement->Data.InsertStatement.valueList =
        parseValueList(tokenListCTX);

    consumeToken(tokenListCTX->indexPosition->type, TOKEN_RPAREN, tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_SEMICOLON,
                 tokenListCTX);
    return insertStatement;
};
