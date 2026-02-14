#include "parser_utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

const char *tokenTypeToString(TokenType s) {
    return (s >= 0) ? tokens[s] : "UNKNOWN";
};

void syntaxError(const char *message) {
    printf("%s", message);
    return;
};

void showIndex(tokenListCTX *tokenListCTX) {
    printf("\nIndex Position: %ld",
           tokenListCTX->indexPosition - tokenListCTX->tokenList);
}

void advance(tokenListCTX *tokenListCTX) { tokenListCTX->indexPosition += 1; };

void decrement(tokenListCTX *tokenListCTX) {
    tokenListCTX->indexPosition -= 1;
};

bool check(size_t tokenType, size_t tokenTypeToBeCheckedAgainst) {
    if (tokenType == tokenTypeToBeCheckedAgainst) {
        return true;
    }
    return false;
};

Token peekToken(tokenListCTX *tokenListCTX) {
    return *tokenListCTX->indexPosition;
};

void consumeToken(size_t tokenType, size_t tokenTypeToBeChecked,
                  tokenListCTX *tokenListCTX) {
    if (check(tokenType, tokenTypeToBeChecked)) {
        advance(tokenListCTX);
        return;
    } else {
        // Build a detailed error message
        char error_msg[256];
        snprintf(error_msg, sizeof(error_msg),
                 "\nERROR:\n  Expected: %s\n  But got: %s\n  On line %ld",
                 tokenTypeToString(tokenTypeToBeChecked),
                 tokenTypeToString(tokenType),
                 tokenListCTX->indexPosition->line);

        syntaxError(error_msg);
        exit(EXIT_FAILURE);
        return;
    }
}

void destroyASTNode(ASTNode *node) {
    if (!node)
        return;

    switch (node->NodeType) {
    case AST_EXIT: {
        free(node);
        break;
    }
    case AST_SELECT: {
        // INFO: only have select list done
        if (node->Data.SelectStatement.selectList) {
            destroyASTNode(node->Data.SelectStatement.selectList);
        }

        if (node->Data.SelectStatement.tableList) {
            destroyASTNode(node->Data.SelectStatement.tableList);
        }

        if (node->Data.SelectStatement.whereClause) {
            destroyASTNode(node->Data.SelectStatement.whereClause);
        }

        if (node->Data.SelectStatement.orderByClause) {
            destroyASTNode(node->Data.SelectStatement.orderByClause);
        }

        free(node);
        break;
    }
    case AST_ORDER_CLAUSE: {
        destroyASTNode(node->Data.OrderClause.qualifiedIdentifier);
        if (node->Data.OrderClause.sortOrder) {
            destroyASTNode(node->Data.OrderClause.sortOrder);
        }

        ASTNode *current = node->next;
        while (current) {
            ASTNode *next = current->next;
            destroyASTNode(current->Data.OrderClause.qualifiedIdentifier);
            free(current);
            current = next;
        }

        free(node);
        break;
    }
    case AST_SORT_ORDER: {
        free(node);
        break;
    }
    case AST_TABLE_LIST: {
        free(node);
        break;
    }
    case AST_SELECT_LIST: {
        destroyASTNode(node->Data.SelectList.simpleExpression);

        ASTNode *current = node->next;
        while (current) {
            ASTNode *next = current->next;
            destroyASTNode(current->Data.SelectList.simpleExpression);
            free(current);
            current = next;
        }
        free(node);
        break;
    }
    case AST_SIMPLE_EXPRESSION: {
        destroyASTNode(node->Data.SimpleExpression.term);

        ASTNode *current = node->Data.SimpleExpression.additiveOperator;
        while (current) {
            ASTNode *next = current->next;
            destroyASTNode(current);
            current = next;
        }
        free(node);
        break;
    }
    case AST_ADDITIVE_OPERATOR: {
        destroyASTNode(node->Data.AdditiveOperator.term);
        free(node);
        break;
    }
    case AST_TERM: {
        destroyASTNode(node->Data.Term.factor);

        ASTNode *current = node->Data.Term.multiplicativeOperator;
        while (current) {
            ASTNode *next = current->next;
            destroyASTNode(current);
            current = next;
        }

        free(node);
        break;
    }
    case AST_MULTIPLICATIVE_OPERATOR: {
        destroyASTNode(node->Data.MultiplicativeOperator.factor);
        free(node);
        break;
    }
    case AST_FACTOR: {
        if (node->Data.Factor.qualifiedIdentifier) {
            destroyASTNode(node->Data.Factor.qualifiedIdentifier);
        } else if (node->Data.Factor.literal) {
            destroyASTNode(node->Data.Factor.literal);
        } else if (node->Data.Factor.functionCall) {
            destroyASTNode(node->Data.Factor.functionCall);
        } else if (node->Data.Factor.simpleExpression) {
            destroyASTNode(node->Data.Factor.simpleExpression);
        }
        free(node);
        break;
    }
    case AST_QUALIFIED_IDENTIFIER: {
        free(node);
        break;
    }
    case AST_LITERAL: {
        free(node);
        break;
    }
    case AST_FUNCTION_CALL: {
        // TODO: implement freeing of arg list
        if (node->Data.FunctionCall.argList) {
            destroyASTNode(node->Data.FunctionCall.argList);
        }
        free(node);
        break;
    }
    case AST_ARG_LIST: {
        destroyASTNode(node->Data.ArgList.simpleExpression);

        ASTNode *current = node->next;
        while (current) {
            ASTNode *next = current->next;
            destroyASTNode(current->Data.ArgList.simpleExpression);
            free(current);
            current = next;
        }
        free(node);
        break;
    }
    case AST_WHERE_CLAUSE: {
        destroyASTNode(node->Data.WhereClause.condition);
        free(node);
        break;
    }
    case AST_CONDITION: {
        destroyASTNode(node->Data.Condition.orCondition);
        free(node);
        break;
    }
    case AST_OR_CONDITION: {
        destroyASTNode(node->Data.OrCondition.andCondition);

        ASTNode *current = node->next;
        while (current) {
            ASTNode *next = current->next;
            destroyASTNode(current->Data.OrCondition.andCondition);
            free(current);
            current = next;
        }
        free(node);
        break;
    }
    case AST_AND_CONDITION: {
        destroyASTNode(node->Data.AndCondition.booleanFactor);

        ASTNode *current = node->next;
        while (current) {
            ASTNode *next = current->next;
            destroyASTNode(current->Data.AndCondition.booleanFactor);
            free(current);
            current = next;
        }
        free(node);
        break;
    }
    case AST_BOOLEAN_FACTOR: {
        if (node->Data.BooleanFactor.comparison) {
            destroyASTNode(node->Data.BooleanFactor.comparison);
        } else if (node->Data.BooleanFactor.condition) {
            destroyASTNode(node->Data.BooleanFactor.condition);
        }
        free(node);
        break;
    }
    case AST_COMPARISON: {
        destroyASTNode(node->Data.Comparison.simpleExpressionL);
        destroyASTNode(node->Data.Comparison.comparisonOperator);
        destroyASTNode(node->Data.Comparison.simpleExpressionR);
        free(node);
        break;
    }
    case AST_COMPARISON_OPERATOR: {
        free(node);
        break;
    }
    }
};

bool containsCondition(tokenListCTX *tokenListCTX) {
    Token *originalposition = tokenListCTX->indexPosition;
    int depth = 0;

    if (peekToken(tokenListCTX).type == TOKEN_LPAREN) {
        depth += 1;
    }
    advance(tokenListCTX);

    while (depth > 0 && peekToken(tokenListCTX).type != TOKEN_EOF) {
        if (peekToken(tokenListCTX).type == TOKEN_LPAREN) {
            depth += 1;
        } else if (peekToken(tokenListCTX).type == TOKEN_RPAREN) {
            depth -= 1;
        } else if (peekToken(tokenListCTX).type == TOKEN_OPERATOR_EQ ||
                   peekToken(tokenListCTX).type == TOKEN_OPERATOR_NEQ ||
                   peekToken(tokenListCTX).type == TOKEN_OPERATOR_LT ||
                   peekToken(tokenListCTX).type == TOKEN_OPERATOR_LTE ||
                   peekToken(tokenListCTX).type == TOKEN_OPERATOR_GT ||
                   peekToken(tokenListCTX).type == TOKEN_OPERATOR_GTE ||
                   peekToken(tokenListCTX).type == TOKEN_KEYWORD_AND ||
                   peekToken(tokenListCTX).type == TOKEN_KEYWORD_OR) {
            tokenListCTX->indexPosition = originalposition;
            return true;
        }
        advance(tokenListCTX);
    }
    tokenListCTX->indexPosition = originalposition;
    return false;
};
