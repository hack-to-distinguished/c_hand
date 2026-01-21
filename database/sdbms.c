#include "input_buffer.h"
#include <stdbool.h>
#include <stddef.h>

int main() {

    while (true) {
        inputBuffer *iPL = createInputBuffer();
        getInput(iPL);

        processInput(iPL);
        destroyInputBuffer(iPL);
    }

    return 0;
}
