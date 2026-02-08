#include <stdbool.h>
typedef struct ASTNode ASTNode;

struct ASTNode {

    enum {
        AST_SELECT,
        AST_SELECT_LIST,
        AST_SIMPLE_EXPRESSION,
        AST_TERM,
        AST_ADDITIVE_OPERATOR,
        AST_MULTIPLICATIVE_OPERATOR,
        AST_FACTOR,
        AST_QUALIFIED_IDENTIFIER,
        AST_LITERAL,
        AST_FUNCTION_CALL,
        AST_ARG_LIST,
        AST_UPDATE,
        AST_DELETE,
        AST_EXIT
    } NodeType;

    union {
        struct {
            bool selectAll;
            ASTNode *selectList;
            ASTNode *tableList;
            ASTNode *whereClause;
            ASTNode *orderByClause;
        } SelectStatement;

        struct {
            ASTNode *simpleExpression;
        } SelectList;

        struct {
            ASTNode *term;
            ASTNode *additiveOperator;
        } SimpleExpression;

        struct {
            ASTNode *factor;
            ASTNode *multiplicativeOperator;
        } Term;

        struct {
            ASTNode *qualifiedIdentifier;
            ASTNode *literal;
            ASTNode *functionCall;
            ASTNode *simpleExpression;
        } Factor;

        struct {
            // INFO: arg list may need a pointer?
            ASTNode *argList;
        } FunctionCall;

        struct {
            ASTNode *simpleExpression;
        } ArgList;

        struct {
            // INFO: dont need anything here, its just tokens
        } Literal;

        struct {
            // INFO: dont need anything here, its just tokens
        } QualifiedIdentifier;

        struct {
            ASTNode *factor;
        } MultiplicativeOperator;

        struct {
            ASTNode *term;
        } AdditiveOperator;

        struct {
            // INFO: Nothing required here
        } ExitStatement;

    } Data;
    ASTNode *next;
};
