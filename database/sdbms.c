#include "input_buffer.h"
#include <stdbool.h>
#include <stddef.h>

int main() {

    while (true) {
        inputBuffer *userInput = createInputBuffer();
        getInput(userInput);

        processInput(userInput);
        destroyInputBuffer(userInput);
    }

    return 0;
}
