#include <stdbool.h>
#include <stddef.h>

#define TOKEN_LIST                                                             \
    X(TOKEN_KEYWORD_SELECT)                                                    \
    X(TOKEN_KEYWORD_FROM)                                                      \
    X(TOKEN_KEYWORD_WHERE)                                                     \
    X(TOKEN_KEYWORD_INSERT)                                                    \
    X(TOKEN_KEYWORD_INTO)                                                      \
    X(TOKEN_KEYWORD_VALUES)                                                    \
    X(TOKEN_KEYWORD_UPDATE)                                                    \
    X(TOKEN_KEYWORD_SET)                                                       \
    X(TOKEN_KEYWORD_DELETE)                                                    \
    X(TOKEN_KEYWORD_EXIT)                                                      \
    X(TOKEN_KEYWORD_NULL)                                                      \
    X(TOKEN_KEYWORD_AND)                                                       \
    X(TOKEN_KEYWORD_OR)                                                        \
    X(TOKEN_KEYWORD_ORDER)                                                     \
    X(TOKEN_KEYWORD_BY)                                                        \
    X(TOKEN_KEYWORD_ASC)                                                       \
    X(TOKEN_KEYWORD_DESC)                                                      \
    X(TOKEN_KEYWORD_AS)                                                        \
    X(TOKEN_KEYWORD_CREATE)                                                    \
    X(TOKEN_KEYWORD_TABLE)                                                     \
    X(TOKEN_KEYWORD_UNIQUE)                                                    \
    X(TOKEN_KEYWORD_INDEX)                                                     \
    X(TOKEN_KEYWORD_CONSTRAINT)                                                \
    X(TOKEN_KEYWORD_INT)                                                       \
    X(TOKEN_KEYWORD_INTEGER)                                                   \
    X(TOKEN_KEYWORD_FLOAT)                                                     \
    X(TOKEN_KEYWORD_DOUBLE)                                                    \
    X(TOKEN_KEYWORD_BOOLEAN)                                                   \
    X(TOKEN_KEYWORD_DATE)                                                      \
    X(TOKEN_KEYWORD_TEXT)                                                      \
    X(TOKEN_KEYWORD_VARCHAR)                                                   \
    X(TOKEN_KEYWORD_CHAR)                                                      \
    X(TOKEN_KEYWORD_DECIMAL)                                                   \
    X(TOKEN_KEYWORD_NOT)                                                       \
    X(TOKEN_KEYWORD_PRIMARY)                                                   \
    X(TOKEN_KEYWORD_DEFAULT)                                                   \
    X(TOKEN_KEYWORD_REFERENCES)                                                \
    X(TOKEN_KEYWORD_KEY)                                                       \
    X(TOKEN_KEYWORD_FOREIGN)                                                   \
    X(TOKEN_KEYWORD_CHECK)                                                     \
    X(TOKEN_IDENTIFIER)                                                        \
    X(TOKEN_STRING_LITERAL)                                                    \
    X(TOKEN_INTEGER_LITERAL)                                                   \
    X(TOKEN_FLOAT_LITERAL)                                                     \
    X(TOKEN_OPERATOR_EQ)                                                       \
    X(TOKEN_OPERATOR_NEQ)                                                      \
    X(TOKEN_OPERATOR_LT)                                                       \
    X(TOKEN_OPERATOR_LTE)                                                      \
    X(TOKEN_OPERATOR_GT)                                                       \
    X(TOKEN_OPERATOR_GTE)                                                      \
    X(TOKEN_OPERATOR_PLUS)                                                     \
    X(TOKEN_OPERATOR_MINUS)                                                    \
    X(TOKEN_OPERATOR_STAR)                                                     \
    X(TOKEN_OPERATOR_SLASH)                                                    \
    X(TOKEN_OPERATOR_LIKE)                                                     \
    X(TOKEN_COMMA)                                                             \
    X(TOKEN_SEMICOLON)                                                         \
    X(TOKEN_LPAREN)                                                            \
    X(TOKEN_RPAREN)                                                            \
    X(TOKEN_DOT)                                                               \
    X(TOKEN_EOF)

typedef enum {
#define X(token) token,
    TOKEN_LIST
#undef X
} TokenType;

typedef struct Token {
    TokenType type;
    const char *lexeme;
    size_t line;
    struct Token *self;
    bool allocatedMem;
} Token;

typedef struct {
    const char *keyword;
    TokenType type;
} Keyword;

static const char *tokens[] = {
#define X(token) #token,
    TOKEN_LIST
#undef X
};

const char *tokenTypeToString(TokenType s);

static Keyword keywords[] = {{"SELECT", TOKEN_KEYWORD_SELECT},
                             {"INSERT", TOKEN_KEYWORD_INSERT},
                             {"DELETE", TOKEN_KEYWORD_DELETE},
                             {"FROM", TOKEN_KEYWORD_FROM},
                             {"UPDATE", TOKEN_KEYWORD_UPDATE},
                             {"WHERE", TOKEN_KEYWORD_WHERE},
                             {"EXIT", TOKEN_KEYWORD_EXIT},
                             {"NULL", TOKEN_KEYWORD_NULL},
                             {"AND", TOKEN_KEYWORD_AND},
                             {"OR", TOKEN_KEYWORD_OR},
                             {"ORDER", TOKEN_KEYWORD_ORDER},
                             {"BY", TOKEN_KEYWORD_BY},
                             {"ASC", TOKEN_KEYWORD_ASC},
                             {"DESC", TOKEN_KEYWORD_DESC},
                             {"INTO", TOKEN_KEYWORD_INTO},
                             {"VALUES", TOKEN_KEYWORD_VALUES},
                             {"SET", TOKEN_KEYWORD_SET},
                             {"LIKE", TOKEN_OPERATOR_LIKE},
                             {"TABLE", TOKEN_KEYWORD_TABLE},
                             {"AS", TOKEN_KEYWORD_AS},
                             {"UNIQUE", TOKEN_KEYWORD_UNIQUE},
                             {"INDEX", TOKEN_KEYWORD_INDEX},
                             {"CONSTRAINT", TOKEN_KEYWORD_CONSTRAINT},
                             {"CREATE", TOKEN_KEYWORD_CREATE},
                             {"INT", TOKEN_KEYWORD_INT},
                             {"INTEGER", TOKEN_KEYWORD_INTEGER},
                             {"FLOAT", TOKEN_KEYWORD_FLOAT},
                             {"DOUBLE", TOKEN_KEYWORD_DOUBLE},
                             {"BOOLEAN", TOKEN_KEYWORD_BOOLEAN},
                             {"DATE", TOKEN_KEYWORD_DATE},
                             {"TEXT", TOKEN_KEYWORD_TEXT},
                             {"VARCHAR", TOKEN_KEYWORD_VARCHAR},
                             {"CHAR", TOKEN_KEYWORD_CHAR},
                             {"DECIMAL", TOKEN_KEYWORD_DECIMAL},
                             {"NOT", TOKEN_KEYWORD_NOT},
                             {"PRIMARY", TOKEN_KEYWORD_PRIMARY},
                             {"DEFAULT", TOKEN_KEYWORD_DEFAULT},
                             {"REFERENCES", TOKEN_KEYWORD_REFERENCES},
                             {"KEY", TOKEN_KEYWORD_KEY},
                             {"FOREIGN", TOKEN_KEYWORD_FOREIGN},
                             {"CHECK", TOKEN_KEYWORD_CHECK}};
