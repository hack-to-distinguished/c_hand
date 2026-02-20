#include "../parser_utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseAliasedExpression(tokenListCTX *tokenListCTX) {
    ASTNode *aliasedExpression = malloc(sizeof(ASTNode));

    if (!aliasedExpression) {
        perror("Memory allocation failed for select list.");
        free(aliasedExpression);
        exit(EXIT_FAILURE);
        return NULL;
    }

    aliasedExpression->NodeType = AST_ALIASED_EXPRESSION;
    aliasedExpression->Data.AliasedExpression.simpleExpression = NULL;
    aliasedExpression->next = NULL;

    aliasedExpression->Data.AliasedExpression.simpleExpression =
        parseSimpleExpression(tokenListCTX);

    if (peekToken(tokenListCTX).type == TOKEN_KEYWORD_AS) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_AS,
                     tokenListCTX);
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_IDENTIFIER,
                     tokenListCTX);
    }

    return aliasedExpression;
};
