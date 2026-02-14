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

static Keyword keywords[] = {
    {"SELECT", TOKEN_KEYWORD_SELECT}, {"INSERT", TOKEN_KEYWORD_INSERT},
    {"DELETE", TOKEN_KEYWORD_DELETE}, {"FROM", TOKEN_KEYWORD_FROM},
    {"UPDATE", TOKEN_KEYWORD_UPDATE}, {"WHERE", TOKEN_KEYWORD_WHERE},
    {"EXIT", TOKEN_KEYWORD_EXIT},     {"NULL", TOKEN_KEYWORD_NULL},
    {"AND", TOKEN_KEYWORD_AND},       {"OR", TOKEN_KEYWORD_OR},
    {"ORDER", TOKEN_KEYWORD_ORDER},   {"BY", TOKEN_KEYWORD_BY},
    {"ASC", TOKEN_KEYWORD_ASC},       {"DESC", TOKEN_KEYWORD_DESC},
    {"INTO", TOKEN_KEYWORD_INTO},     {"VALUES", TOKEN_KEYWORD_VALUES},
    {"SET", TOKEN_KEYWORD_SET}

};
