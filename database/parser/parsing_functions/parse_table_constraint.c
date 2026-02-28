#include "../parser_utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseTableConstraint(tokenListCTX *tokenListCTX) {
    ASTNode *tableConstraint = malloc(sizeof(ASTNode));

    if (!tableConstraint) {
        perror("Memory allocation failed for exit statement.");
        free(tableConstraint);
        exit(EXIT_FAILURE);
        return NULL;
    }

    tableConstraint->NodeType = AST_TABLE_CONSTRAINT;
    tableConstraint->next = NULL;
    tableConstraint->Data.TableConstraint.tableConstraintType = NULL;

    Token nextToken = peekToken(tokenListCTX);

    if (nextToken.type == TOKEN_KEYWORD_CONSTRAINT) {
        consumeToken(tokenListCTX->indexPosition->type,
                     TOKEN_KEYWORD_CONSTRAINT, tokenListCTX);
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_IDENTIFIER,
                     tokenListCTX);
    }

    tableConstraint->Data.TableConstraint.tableConstraintType =
        parseTableConstraintType(tokenListCTX);

    return tableConstraint;
};
