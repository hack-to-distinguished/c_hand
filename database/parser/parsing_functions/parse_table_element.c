#include "../parser_utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseTableElement(tokenListCTX *tokenListCTX) {
    ASTNode *tableElement = malloc(sizeof(ASTNode));

    if (!tableElement) {
        perror("Memory allocation failed for exit statement.");
        free(tableElement);
        exit(EXIT_FAILURE);
        return NULL;
    }

    tableElement->NodeType = AST_TABLE_ELEMENT;
    tableElement->Data.TableElement.tableConstraint = NULL;
    tableElement->Data.TableElement.columnDefinition = NULL;
    tableElement->next = NULL;

    if (peekToken(tokenListCTX).type == TOKEN_IDENTIFIER) {
        tableElement->Data.TableElement.columnDefinition =
            parseColumnDefinition(tokenListCTX);
    } else {
        // INFO: make this else case more specific
        tableElement->Data.TableElement.tableConstraint =
            parseTableConstraint(tokenListCTX);
    }

    return tableElement;
};
