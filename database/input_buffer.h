#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *buffer;
} inputBuffer;

inputBuffer *createInputBuffer();
void destroyInputBuffer(inputBuffer *inputBuffer);
void getInput(inputBuffer *inputBuffer);
void processInput(inputBuffer *inputBuffer);
