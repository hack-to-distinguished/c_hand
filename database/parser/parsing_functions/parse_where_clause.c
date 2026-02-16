#include "../parser_utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseWhereClause(tokenListCTX *tokenListCTX) {
    ASTNode *whereClause = malloc(sizeof(ASTNode));

    if (!whereClause) {
        perror("Memory allocation failed for exit statement.");
        free(whereClause);
        exit(EXIT_FAILURE);
        return NULL;
    }

    whereClause->Data.WhereClause.condition = NULL;
    whereClause->next = NULL;
    whereClause->NodeType = AST_WHERE_CLAUSE;

    consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_WHERE,
                 tokenListCTX);
    whereClause->Data.WhereClause.condition = parseCondition(tokenListCTX);

    return whereClause;
};
