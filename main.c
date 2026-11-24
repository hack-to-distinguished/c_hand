#include <stdio.h>
#include "pirradazis/bindings/rust-in-c.h"

int main() {

    uint8_t data[] = {0, 1, 2, 3, 4, 5, 6};
    size_t data_length = 7;

    uint32_t hash = crc32(data, data_length);
    printf("Hash: %u\n", hash);

    return 0;
}
