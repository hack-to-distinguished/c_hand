#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

void say_hello(void);

/**
 * Calculate CRC32 for passed data. If len is non-zero, data must point to a valid slice in memory of length len.
 *
 * # Safety
 * This function uses [std::slice::from_raw_parts] to create a slice
 * out of the passed raw pointer and length, and
 * this function exhibits Undefined Behavior in the same cases as
 * `from_raw_parts`
 */
uint32_t crc32(const uint8_t *data,
               uintptr_t len);
