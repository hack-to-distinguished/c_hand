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
    bool seenLastSemicolon = false;
    char *inputFromUser = (char *)malloc(sizeof(char) * bufferSize);

    if (!inputFromUser) {
        perror("failed to allocate memory");
        free(inputFromUser);
        exit(EXIT_FAILURE);
    }

    int c;
    printf("\ndb > ");
    fflush(stdout);

    while ((c = getchar()) != EOF) {
        if (bufferIndex >= bufferSize - 1) {
            bufferSize *= 2;
            inputFromUser = (char *)realloc(inputFromUser, bufferSize);
            if (!inputFromUser) {
                perror("failed to reallocate memory");
                free(inputFromUser);
                exit(EXIT_FAILURE);
            }
        }
        inputFromUser[bufferIndex] = c;
        bufferIndex++;

        if (c == ';') {
            seenLastSemicolon = true;
        } else if (c != '\n') {
            seenLastSemicolon = false;
        }

        if (c == '\n') {
            if (seenLastSemicolon) {
                break;
            }
            printf("   > ");
            fflush(stdout);
        }
    }

    inputFromUser[bufferIndex] = '\0';

    char *lastSemicolon = strrchr(inputFromUser, ';');
    if (lastSemicolon) {
        lastSemicolon[1] = '\0';
    }
    ptrInputBuffer->buffer = inputFromUser;
    return;
}

void processInput(inputBuffer *ptrInputBuffer) {
    scanTokens(ptrInputBuffer->buffer);
    return;
}
