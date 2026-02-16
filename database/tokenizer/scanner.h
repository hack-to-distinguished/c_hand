// #include "token_list.h"
#include "../parser/parser_utils.h"
#include <stdbool.h>

void scanTokens(char *buffer);
char *scanToken(char *currentPosOfLexeme, tokenListCTX *ctx, char *bufferStart,
                size_t lineNumber);
void addToken(tokenListCTX *ctx, TokenType tokenType, char *lexeme,
              size_t lineNumber);
bool isAtEnd(char *posInBuffer);
bool matchChar(char *currentPosOfLexeme, char expectedChar);
char *stringLiteral(char *currentPosOfLexeme);
char *getStringLiteral(char *currentPosOfLexeme, char *startOfLexeme);
bool isDigit(char c);
char *numberLiteral(char *currentPosOfLexeme, bool isNegative);
char *getNumberLiteral(char *currentPosOfLexeme, char *startOfLexeme,
                       bool isNegative);
bool checkFloat(char *numberLiteral);
bool isAlpha(char c);
bool isAlphaNumeric(char c);
char *identifier(char *currentPosOfLexeme);
char *getIdentifierLiteral(char *currentPosOfLexeme, char *startOfLexeme);
void capitaliseString(char *str);
