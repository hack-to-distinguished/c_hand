#include "scanner.h"
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void scanTokens(char *buffer) {
    tokenListCTX *ctx = initialiseTokenList(8);
    char *startOfLexeme = buffer;
    char *currentPosOfLexeme = startOfLexeme;
    size_t lineNumber = 1;

    // scan the actual tokens
    while (!isAtEnd(currentPosOfLexeme)) {
        startOfLexeme = currentPosOfLexeme;
        if (*startOfLexeme == '\n') {
            lineNumber += 1;
        }
        currentPosOfLexeme =
            scanToken(currentPosOfLexeme, ctx, startOfLexeme, lineNumber);
    }

    addToken(ctx, TOKEN_EOF, "EOF", lineNumber);
    printAllTokens(ctx);
    parse(ctx);
    destroyTokenList(ctx);
    return;
}

char *scanToken(char *currentPosOfLexeme, tokenListCTX *ctx, char *bufferStart,
                size_t lineNumber) {
    char c = *currentPosOfLexeme;

    switch (c) {
        // Skip whitespace
    case ' ':
    case '\r':
    case '\t':
    case '\n':
        break;
    // Punctuation
    case ',':
        addToken(ctx, TOKEN_COMMA, ",", lineNumber);
        break;
    case ';':
        addToken(ctx, TOKEN_SEMICOLON, ";", lineNumber);
        break;
    case '(':
        addToken(ctx, TOKEN_LPAREN, "(", lineNumber);
        break;
    case ')':
        addToken(ctx, TOKEN_RPAREN, ")", lineNumber);
        break;
    case '.':
        addToken(ctx, TOKEN_DOT, ".", lineNumber);
        break;

    // Operators
    case '!':
        if (matchChar(currentPosOfLexeme, '=')) {
            currentPosOfLexeme += 1;
            addToken(ctx, TOKEN_OPERATOR_NEQ, "!=", lineNumber);
            break;
        } else {
            fprintf(stderr, "\nUnrecognised Input");
            exit(1);
            break;
        }
    case '=':
        addToken(ctx, TOKEN_OPERATOR_EQ, "=", lineNumber);
        break;
    case '<':
        if (matchChar(currentPosOfLexeme, '=')) {
            currentPosOfLexeme += 1;
            addToken(ctx, TOKEN_OPERATOR_LTE, "<=", lineNumber);
        } else {
            addToken(ctx, TOKEN_OPERATOR_LT, "<", lineNumber);
        }
        break;
    case '>':
        if (matchChar(currentPosOfLexeme, '=')) {
            currentPosOfLexeme += 1;
            addToken(ctx, TOKEN_OPERATOR_GTE, ">=", lineNumber);
        } else {
            addToken(ctx, TOKEN_OPERATOR_GT, ">", lineNumber);
        }
        break;
    case '+':
        addToken(ctx, TOKEN_OPERATOR_PLUS, "+", lineNumber);
        break;
    case '-':
        if (!isDigit(*(currentPosOfLexeme - 1)) &&
            isDigit(*(currentPosOfLexeme + 1))) {
            currentPosOfLexeme = numberLiteral(currentPosOfLexeme, true);
            char *numberLiteral =
                getNumberLiteral(currentPosOfLexeme, bufferStart, true);
            if (checkFloat(numberLiteral)) {
                addToken(ctx, TOKEN_FLOAT_LITERAL, numberLiteral, lineNumber);
            } else {
                addToken(ctx, TOKEN_INTEGER_LITERAL, numberLiteral, lineNumber);
            }
            currentPosOfLexeme -= 1;
        } else {
            addToken(ctx, TOKEN_OPERATOR_MINUS, "-", lineNumber);
        }
        break;
    case '*':
        addToken(ctx, TOKEN_OPERATOR_STAR, "*", lineNumber);
        break;
    case '/':
        addToken(ctx, TOKEN_OPERATOR_SLASH, "/", lineNumber);
        break;

    // STRING LITERALS
    case '\'':
        currentPosOfLexeme = stringLiteral(currentPosOfLexeme);
        addToken(ctx, TOKEN_STRING_LITERAL,
                 getStringLiteral(currentPosOfLexeme, bufferStart), lineNumber);
        break;
    default:
        // INTEGER + FLOAT LITERALS
        if (isDigit(c)) {
            currentPosOfLexeme = numberLiteral(currentPosOfLexeme, false);
            char *numberLiteral =
                getNumberLiteral(currentPosOfLexeme, bufferStart, false);
            if (checkFloat(numberLiteral)) {
                addToken(ctx, TOKEN_FLOAT_LITERAL, numberLiteral, lineNumber);
            } else {
                addToken(ctx, TOKEN_INTEGER_LITERAL, numberLiteral, lineNumber);
            }
            currentPosOfLexeme -= 1;
            // assuming that any identifier will begin with a character
        } else if (isAlpha(c)) {
            currentPosOfLexeme = identifier(currentPosOfLexeme);

            char *lexeme =
                getIdentifierLiteral(currentPosOfLexeme, bufferStart);

            capitaliseString(lexeme);

            bool found = false;
            for (int i = 0; i < (sizeof(keywords) / sizeof(Keyword)); i++) {
                if (strcmp(keywords[i].keyword, lexeme) == 0) {
                    addToken(ctx, keywords[i].type, lexeme, lineNumber);
                    found = true;
                    break;
                }
            }

            if (!found) {
                free(lexeme);
                char *lexemeIdentifier =
                    getIdentifierLiteral(currentPosOfLexeme, bufferStart);
                addToken(ctx, TOKEN_IDENTIFIER, lexemeIdentifier, lineNumber);
            }
            currentPosOfLexeme -= 1;
        } else {
            fprintf(stderr, "\nUnrecognised Input");
            destroyTokenList(ctx);
            exit(1);
        }
        break;
    }

    return (currentPosOfLexeme + 1);
};

void addToken(tokenListCTX *ctx, TokenType tokenType, char *lexeme,
              size_t lineNumber) {
    Token *token = malloc(sizeof(Token));
    token->type = tokenType;
    token->lexeme = lexeme;
    token->self = token;
    token->line = lineNumber;
    appendToken(token, ctx);
    return;
};

bool isAtEnd(char *posInBuffer) {
    if (*posInBuffer == '\0' || *posInBuffer == '\r') {
        return true;
    }
    return false;
};

bool matchChar(char *posInBuffer, char expectedChar) {
    posInBuffer += 1;
    if (*posInBuffer == expectedChar) {
        return true;
    }
    return false;
};

char *stringLiteral(char *currentPosOfLexeme) {
    currentPosOfLexeme += 1;
    while (!isAtEnd(currentPosOfLexeme) && currentPosOfLexeme[0] != '\'') {
        currentPosOfLexeme += 1;
    }

    if (isAtEnd(currentPosOfLexeme)) {
        fprintf(stderr, "\nUnterminated string literal");
        exit(0);
    }
    return currentPosOfLexeme;
}

char *getStringLiteral(char *currentPosOfLexeme, char *startOfLexeme) {
    size_t len = (&currentPosOfLexeme[0] - startOfLexeme);
    currentPosOfLexeme -= len;
    char *string = malloc(sizeof(char) * len);
    size_t index = 0;
    currentPosOfLexeme += 1;
    while (currentPosOfLexeme[0] != '\'') {
        string[index] = currentPosOfLexeme[0];
        currentPosOfLexeme += 1;
        index += 1;
    }
    string[index] = '\0';
    return string;
};

bool isDigit(char c) {
    if (c >= '0' && c <= '9') {
        return true;
    }
    return false;
};

char *numberLiteral(char *currentPosOfLexeme, bool isNegative) {
    if (isNegative) {
        while (!isAtEnd(currentPosOfLexeme) &&
               (isDigit(currentPosOfLexeme[0]) ||
                currentPosOfLexeme[0] == '.' || currentPosOfLexeme[0] == '-')) {
            currentPosOfLexeme += 1;
        }
    } else {
        while (
            !isAtEnd(currentPosOfLexeme) &&
            (isDigit(currentPosOfLexeme[0]) || currentPosOfLexeme[0] == '.')) {
            currentPosOfLexeme += 1;
        }
    }

    if (currentPosOfLexeme[0] == '.') {
        fprintf(stderr, "\nUnrecognised number format.");
        exit(0);
    }

    return currentPosOfLexeme;
};

char *getNumberLiteral(char *currentPosOfLexeme, char *startOfLexeme,
                       bool isNegative) {
    size_t len = (&currentPosOfLexeme[0] - startOfLexeme) + 1;
    currentPosOfLexeme -= len - 1;
    char *number = malloc(sizeof(char) * len);
    size_t index = 0;
    size_t numOfDecimals = 0;
    if (isNegative) {
        while (isDigit(currentPosOfLexeme[0]) || currentPosOfLexeme[0] == '.' ||
               currentPosOfLexeme[0] == '-') {
            if (currentPosOfLexeme[0] == '.') {
                numOfDecimals += 1;
            }
            number[index] = currentPosOfLexeme[0];
            currentPosOfLexeme += 1;
            index += 1;
        }
    } else {
        while (isDigit(currentPosOfLexeme[0]) || currentPosOfLexeme[0] == '.') {
            if (currentPosOfLexeme[0] == '.') {
                numOfDecimals += 1;
            }
            number[index] = currentPosOfLexeme[0];
            currentPosOfLexeme += 1;
            index += 1;
        }
    }

    number[index] = '\0';
    if (numOfDecimals > 1) {
        fprintf(stderr, "\nUnrecognised number format.");
        exit(0);
    }
    return number;
};

bool checkFloat(char *numberLiteral) {
    for (size_t i = 0; i < strlen(numberLiteral); i++) {
        if (numberLiteral[i] == '.') {
            return true;
        }
    }
    return false;
};

bool isAlpha(char c) {
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
        return true;
    }
    return false;
};

bool isAlphaNumeric(char c) { return isDigit(c) || isAlpha(c); };

char *identifier(char *currentPosOfLexeme) {
    while (isAlphaNumeric(currentPosOfLexeme[0])) {
        currentPosOfLexeme += 1;
    }
    return currentPosOfLexeme;
};

char *getIdentifierLiteral(char *currentPosOfLexeme, char *startOfLexeme) {
    size_t len = (&currentPosOfLexeme[0] - startOfLexeme) + 1;
    currentPosOfLexeme -= len - 1;
    char *identifier = malloc(sizeof(char) * len);
    size_t index = 0;
    while (isAlphaNumeric(currentPosOfLexeme[0])) {
        identifier[index] = currentPosOfLexeme[0];
        currentPosOfLexeme += 1;
        index += 1;
    }
    identifier[index] = '\0';
    return identifier;
};

void capitaliseString(char *str) {
    while (*str) {
        *str = toupper((unsigned char)*str);
        str++;
    }
}
