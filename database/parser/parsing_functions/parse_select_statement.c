#include "../parser_utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseSelectStatement(tokenListCTX *tokenListCTX) {
    printf("\ndean");

    ASTNode *selectStatement = malloc(sizeof(ASTNode));

    if (!selectStatement) {
        perror("Memory allocation failed for exit statement.");
        free(selectStatement);
        exit(EXIT_FAILURE);
        return NULL;
    }

    selectStatement->NodeType = AST_SELECT;
    selectStatement->next = NULL;
    selectStatement->Data.SelectStatement.selectAll = false;
    selectStatement->Data.SelectStatement.tableList = NULL;
    selectStatement->Data.SelectStatement.selectList = NULL;
    selectStatement->Data.SelectStatement.whereClause = NULL;
    selectStatement->Data.SelectStatement.orderByClause = NULL;

    consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_SELECT,
                 tokenListCTX);

    Token nextToken = peekToken(tokenListCTX);
    printf("\ntoken type (next): %s", tokenTypeToString(nextToken.type));
    if (nextToken.type == TOKEN_OPERATOR_STAR) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_OPERATOR_STAR,
                     tokenListCTX);
        selectStatement->Data.SelectStatement.selectAll = true;
    } else if (nextToken.type == TOKEN_IDENTIFIER) {
        // TODO: Create select list parser
        printf("\nselect list");
        selectStatement->Data.SelectStatement.selectList =
            parseSelectList(tokenListCTX);
    }

    consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_FROM,
                 tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_IDENTIFIER,
                 tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_SEMICOLON,
                 tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_EOF, tokenListCTX);

    // TODO: OPTIONAL statements:
    // TODO: Create where clause parser
    // TODO: Create order by clause parser
    // TODO: Create table list parser

    // TODO: If user inputted extra statements, insert corresponding nodes into
    // select statement tree

    return selectStatement;
};
