#include <stdbool.h>
typedef struct ASTNode ASTNode;

struct ASTNode {

    enum { AST_SELECT, AST_INSERT, AST_UPDATE, AST_DELETE, AST_EXIT } NodeType;

    union {
        struct {
            bool selectAll;
            ASTNode *columns;
            ASTNode *tableList;
            ASTNode *selectList;
            ASTNode *whereClause;
            ASTNode *orderByClause;
        } SelectStatement;

        struct {
            // INFO: Nothing required here
        } ExitStatement;

    } Data;
    ASTNode *next;
};
