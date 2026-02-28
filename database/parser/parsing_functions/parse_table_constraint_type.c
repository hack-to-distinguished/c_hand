#include "../parser_utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseTableConstraintType(tokenListCTX *tokenListCTX) {
    ASTNode *tableConstraintType = malloc(sizeof(ASTNode));

    if (!tableConstraintType) {
        perror("Memory allocation failed for exit statement.");
        free(tableConstraintType);
        exit(EXIT_FAILURE);
        return NULL;
    }

    tableConstraintType->NodeType = AST_TABLE_CONSTRAINT_TYPE;
    tableConstraintType->next = NULL;
    tableConstraintType->Data.TableConstraintType.condition = NULL;
    tableConstraintType->Data.TableConstraintType.columnListL = NULL;
    tableConstraintType->Data.TableConstraintType.columnListR = NULL;

    Token nextToken = peekToken(tokenListCTX);

    if (nextToken.type == TOKEN_KEYWORD_PRIMARY) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_PRIMARY,
                     tokenListCTX);
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_KEY,
                     tokenListCTX);
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_LPAREN,
                     tokenListCTX);
        tableConstraintType->Data.TableConstraintType.columnListL =
            parseColumnList(tokenListCTX);
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_RPAREN,
                     tokenListCTX);
    } else if (nextToken.type == TOKEN_KEYWORD_UNIQUE) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_UNIQUE,
                     tokenListCTX);
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_LPAREN,
                     tokenListCTX);
        tableConstraintType->Data.TableConstraintType.columnListL =
            parseColumnList(tokenListCTX);
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_RPAREN,
                     tokenListCTX);
    } else if (nextToken.type == TOKEN_KEYWORD_FOREIGN) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_FOREIGN,
                     tokenListCTX);
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_KEY,
                     tokenListCTX);
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_LPAREN,
                     tokenListCTX);
        tableConstraintType->Data.TableConstraintType.columnListL =
            parseColumnList(tokenListCTX);
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_RPAREN,
                     tokenListCTX);
        consumeToken(tokenListCTX->indexPosition->type,
                     TOKEN_KEYWORD_REFERENCES, tokenListCTX);
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_IDENTIFIER,
                     tokenListCTX);
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_LPAREN,
                     tokenListCTX);
        tableConstraintType->Data.TableConstraintType.columnListR =
            parseColumnList(tokenListCTX);
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_RPAREN,
                     tokenListCTX);
    } else if (nextToken.type == TOKEN_KEYWORD_CHECK) {
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_KEYWORD_CHECK,
                     tokenListCTX);
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_LPAREN,
                     tokenListCTX);
        tableConstraintType->Data.TableConstraintType.condition =
            parseCondition(tokenListCTX);
        consumeToken(tokenListCTX->indexPosition->type, TOKEN_RPAREN,
                     tokenListCTX);
    }

    return tableConstraintType;
};
