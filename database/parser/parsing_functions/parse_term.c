#include "../parser_utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseTerm(tokenListCTX *tokenListCTX) {
    ASTNode *term = malloc(sizeof(ASTNode));

    if (!term) {
        perror("Memory allocation failed for exit statement.");
        free(term);
        exit(EXIT_FAILURE);
        return NULL;
    }

    term->next = NULL;
    term->NodeType = AST_TERM;
    term->Data.Term.factor = NULL;
    term->Data.Term.multiplicativeOperator = NULL;

    term->Data.Term.factor = parseFactor(tokenListCTX);

    ASTNode *tail = NULL;

    while (peekToken(tokenListCTX).type == TOKEN_OPERATOR_STAR ||
           peekToken(tokenListCTX).type == TOKEN_OPERATOR_SLASH) {
        ASTNode *newMultiplicativeOperator =
            parseMultiplicativeOperator(tokenListCTX);

        if (term->Data.Term.multiplicativeOperator == NULL) {
            term->Data.Term.multiplicativeOperator = newMultiplicativeOperator;
            tail = newMultiplicativeOperator;
        } else {
            tail->next = newMultiplicativeOperator;
            tail = tail->next;
        }
    }

    return term;
};
