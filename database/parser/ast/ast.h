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
        AST_TABLE_LIST,
        AST_WHERE_CLAUSE,
        AST_CONDITION,
        AST_OR_CONDITION,
        AST_AND_CONDITION,
        AST_BOOLEAN_FACTOR,
        AST_COMPARISON,
        AST_COMPARISON_OPERATOR,
        AST_ORDER_CLAUSE,
        AST_SORT_ORDER,
        AST_INSERT,
        AST_VALUE_LIST,
        AST_COLUMN_LIST,
        AST_ALIASED_EXPRESSION,
        AST_ALIASED_TABLE,
        AST_UPDATE,
        AST_SET_LIST,
        AST_DELETE,
        AST_CREATE,
        AST_CREATE_BODY,
        AST_CREATE_TABLE,
        AST_TABLE_ELEMENT_LIST,
        AST_TABLE_ELEMENT,
        AST_COLUMN_DEFINITION,
        AST_TABLE_CONSTRAINT,
        AST_TABLE_CONSTRAINT_TYPE,
        AST_DATA_TYPE,
        AST_COLUMN_CONTRAINT,
        AST_COLUMN_CONTRAINT_LIST,
        AST_CREATE_INDEX,
        AST_ALTER,
        AST_ALTER_ACTION,
        AST_ALTER_ADD_BODY,
        AST_ALTER_DROP_BODY,
        AST_DROP,
        AST_DROP_BODY,
        AST_TRUNCATE,
        AST_EXIT
    } NodeType;

    union {

        struct {
            ASTNode *setList;
            ASTNode *whereClause;
        } UpdateStatement;

        struct {
            ASTNode *qualifiedIdentifier;
            ASTNode *simpleExpression;
        } SetList;

        struct {
            ASTNode *whereClause;
        } DeleteStatement;

        struct {
            bool selectAll;
            ASTNode *selectList;
            ASTNode *tableList;
            ASTNode *whereClause;
            ASTNode *orderByClause;
        } SelectStatement;

        struct {
            ASTNode *aliasedExpression;
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

        struct {
            ASTNode *aliasedTable;
        } TableList;

        struct {
            ASTNode *condition;
        } WhereClause;

        struct {
            ASTNode *orCondition;
        } Condition;

        struct {
            ASTNode *andCondition;
        } OrCondition;

        struct {
            ASTNode *booleanFactor;
        } AndCondition;

        struct {
            ASTNode *condition;
            ASTNode *comparison;
        } BooleanFactor;

        struct {
            ASTNode *simpleExpressionL;
            ASTNode *comparisonOperator;
            ASTNode *simpleExpressionR;
        } Comparison;

        struct {
            // INFO: Nothing required here
        } ComparisonOperator;

        struct {
            ASTNode *qualifiedIdentifier;
            ASTNode *sortOrder;
        } OrderClause;

        struct {
            // INFO: Nothing required here
        } SortOrder;

        struct {
            ASTNode *columnList;
            ASTNode *valueList;
        } InsertStatement;

        struct {
        } ColumnList;

        struct {
            ASTNode *simpleExpression;
        } ValueList;

        struct {
            ASTNode *simpleExpression;
        } AliasedExpression;

        struct {
            // INFO: Nothing required here
        } AliasedTable;

        struct {
            ASTNode *createBody;
        } CreateStatement;

        struct {
            ASTNode *createTableStatement;
            ASTNode *createIndexStatement;
        } CreateBody;

        struct {
            ASTNode *columnList;
        } CreateIndexStatement;

        struct {
            ASTNode *tableElementList;
        } CreateTableStatement;

        struct {
            ASTNode *tableElement;
        } TableElementList;

        struct {
            ASTNode *columnDefinition;
            ASTNode *tableConstraint;
        } TableElement;

        struct {
            ASTNode *dataType;
            ASTNode *columnConstraintList;
        } ColumnDefinition;

        struct {
            ASTNode *tableConstraintType;
        } TableConstraint;

        struct {
            ASTNode *condition;
            ASTNode *columnListL;
            ASTNode *columnListR;
        } TableConstraintType;

        struct {
            // INFO: nothing required here
        } DataType;

        struct {
            ASTNode *columnConstraint;
        } ColumnConstraintList;

        struct {
            ASTNode *simpleExpression;
        } ColumnConstraint;

        struct {
            ASTNode *alterAction;
        } AlterStatement;

        struct {
            ASTNode *alterAddBody;
            ASTNode *alterDropBody;
            ASTNode *columnDefinition;
        } AlterAction;

        struct {
            ASTNode *columnDefinition;
            ASTNode *tableConstraint;
        } AlterAddBody;

        struct {
            // INFO: nothing needed here
        } AlterDropBody;

        struct {
            ASTNode *dropBody;
        } DropStatement;

        struct {
            // INFO: nothing needed here
        } DropBody;

        struct {
            // INFO: nothing needed here
        } TruncateStatement;

    } Data;
    ASTNode *next;
};
