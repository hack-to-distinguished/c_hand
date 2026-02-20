#include "../parser_utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseAliasedTable(tokenListCTX *tokenListCTX) {
    ASTNode *aliasedTable = malloc(sizeof(ASTNode));

    if (!aliasedTable) {
        perror("Memory allocation failed for select list.");
        free(aliasedTable);
        exit(EXIT_FAILURE);
        return NULL;
    }

    aliasedTable->NodeType = AST_ALIASED_TABLE;
    aliasedTable->next = NULL;

    consumeToken(tokenListCTX->indexPosition->type, TOKEN_IDENTIFIER,
                 tokenListCTX);

    if (peekToken(tokenListCTX).type == TOKEN_KEYWORD_AS) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_AS,
                     tokenListCTX);
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_IDENTIFIER,
                     tokenListCTX);
    }

    return aliasedTable;
};
