#include <stdbool.h>

// Main SQL Statements
typedef enum {
    SELECT_STATEMENT,
    INSERT_STATEMENT,
    UPDATE_STATEMENT,
    DELETE_STATEMENT,
    EXIT_STATEMENT
} StatementType;

typedef struct {
    StatementType type;
    void *data;
} SQLStatement;

// TODO: fill in all of these structs with necessary information when
// programming their corresponding parse function
// NOTE: void* used as these structures have not yet been defined...
typedef struct {
    // don't need to actually store anything for this struct
} ExitStatement;

typedef struct {
    void *SelectList;
    void *TableList;
    void *WhereClause;
    void *OrderByClause;
} SelectStatement;

typedef struct {
    void *TableName;
    void *ValueList;
    void *ColumnList;
} InsertStatement;

typedef struct {
    void *TableName;
    void *SetList;
    void *WhereClause;
} UpdateStatement;

typedef struct {
    void *TableName;
    void *WhereClause;
} DeleteStatement;
