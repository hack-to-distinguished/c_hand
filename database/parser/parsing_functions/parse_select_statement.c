#include "../parser_utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseSelectStatement(tokenListCTX *tokenListCTX) {

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
    if (nextToken.type == TOKEN_OPERATOR_STAR) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_OPERATOR_STAR,
                     tokenListCTX);
        selectStatement->Data.SelectStatement.selectAll = true;
    } else {
        // TODO: Create select list parser
        selectStatement->Data.SelectStatement.selectList =
            parseSelectList(tokenListCTX);
    }

    consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_FROM,
                 tokenListCTX);

    // INFO: parse table list
    selectStatement->Data.SelectStatement.tableList =
        parseTableList(tokenListCTX);

    if (peekToken(tokenListCTX).type == TOKEN_KEYWORD_WHERE) {
        selectStatement->Data.SelectStatement.whereClause =
            parseWhereClause(tokenListCTX);
    }

    // TODO: OPTIONAL statements:
    // TODO: Create order by clause parser

    consumeToken(tokenListCTX->indexPosition->type, TOKEN_SEMICOLON,
                 tokenListCTX);
    consumeToken(tokenListCTX->indexPosition->type, TOKEN_EOF, tokenListCTX);

    return selectStatement;
};
