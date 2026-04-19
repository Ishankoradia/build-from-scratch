#ifndef LZ77_H
#define LZ77_H

#include <stddef.h>

typedef struct {
    int length;
    int distance;
} LZ77Match;

LZ77Match find_longest_match(unsigned char *input_bytes, size_t input_length, size_t position);

#endif