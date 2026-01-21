#include "input_buffer.h"
#include "tokenizer/scanner.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

inputBuffer *createInputBuffer() {
    inputBuffer *ptrInputLineBuffer = malloc(sizeof(inputBuffer));
    ptrInputLineBuffer->buffer = NULL;
    return ptrInputLineBuffer;
}

void destroyInputBuffer(inputBuffer *ptrInputBuffer) {
    free(ptrInputBuffer->buffer);
    free(ptrInputBuffer);
    return;
}

void getInput(inputBuffer *ptrInputBuffer) {

    size_t bufferSize = 2;
    size_t bufferIndex = 0;
    char *inputFromUser = (char *)malloc(sizeof(char) * bufferSize);

    printf("----------\n");
    printf("db > ");
    for (int character = fgetc(stdin); character != EOF && character != ';';
         character = fgetc(stdin)) {
        if (bufferIndex >= bufferSize) {
            bufferSize *= 2;
            inputFromUser = (char *)realloc(inputFromUser, bufferSize);
        }
        if (character == '\n' && bufferIndex > 0) {
            printf("   > ");
        }
        if (character != '\n') {
            inputFromUser[bufferIndex] = character;
            bufferIndex += 1;
        }
    };

    inputFromUser = (char *)realloc(inputFromUser, bufferIndex + 2);
    inputFromUser[bufferIndex] = ';';
    inputFromUser[bufferIndex + 1] = '\0';

    printf("----------\n");
    ptrInputBuffer->buffer = inputFromUser;
    return;
}

void processInput(inputBuffer *ptrInputBuffer) {
    scanTokens(ptrInputBuffer->buffer);
    return;
}
